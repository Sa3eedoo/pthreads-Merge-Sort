#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#define maxline 1000

int no_of_elements;
int numbers[maxline];

typedef struct iteration
{
    int low;
    int high;
} iteration;

void read_file(char filename[]);

void *sort(void *arg);

void merge(int low, int mid, int high);

int main()
{
    // reading file name from user
    printf("Enter file name: ");

    char filename[maxline];

    fgets(filename, maxline, stdin);
    if ((strlen(filename) > 0) && (filename[strlen(filename) - 1] == '\n'))
        filename[strlen(filename) - 1] = '\0';

    read_file(filename);

    // createing the first thread to run the sort function
    pthread_t th;
    iteration current_iteration;

    current_iteration.low = 0;
    current_iteration.high = no_of_elements - 1;

    pthread_create(&th, NULL, &sort, &current_iteration);

    // joining the the first thread to wait for the whole sorting process to finish
    pthread_join(th, NULL);

    // printing out the numbers after sorting
    printf("Sorted array: ");

    for (int i = 0; i < no_of_elements; i++)
    {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    return 0;
}

void read_file(char filename[])
{
    // opening the file for reading
    FILE *fptr;
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("ERROR: file not found or being used.");
    }
    else
    {
        // reading the size of array of numbers
        char size[maxline];

        fgets(size, maxline, fptr);
        if ((strlen(size) > 0) && (size[strlen(size) - 1] == '\n'))
            size[strlen(size) - 1] = '\0';

        char *ptr;
        ptr = strtok(size, " ");
        no_of_elements = atoi(ptr);

        // reading the numbers and putting them in the array
        char numbers_string[maxline];

        fgets(numbers_string, maxline, fptr);
        if ((strlen(numbers_string) > 0) && (numbers_string[strlen(numbers_string) - 1] == '\n'))
            numbers_string[strlen(numbers_string) - 1] = '\0';

        ptr = strtok(numbers_string, " ");
        int i = 0;

        while (ptr != NULL)
        {
            numbers[i] = atoi(ptr);
            ptr = strtok(NULL, " ");
            i++;
        }
    }
    fclose(fptr);
}

void *sort(void *arg)
{
    // converting parameters
    iteration *current_iteration = arg;
    int low = current_iteration->low;
    int high = current_iteration->high;

    // keep recursion until array of 1 element
    if (low < high)
    {
        // getting the index of the middle of the array
        int mid = (low + high) / 2;

        // creating two threads to recursivley sort each part of the array
        pthread_t th[2];
        iteration current_iteration[2];

        current_iteration[0].low = low;
        current_iteration[0].high = mid;

        current_iteration[1].low = mid + 1;
        current_iteration[1].high = high;

        pthread_create(&th[0], NULL, &sort, &current_iteration[0]);
        pthread_create(&th[1], NULL, &sort, &current_iteration[1]);

        //printf("thread no.: %ld\n", th[0]);
        //printf("thread no.: %ld\n", th[1]);

        // joining the two threads created to wait for the other to finish dividing
        pthread_join(th[0], NULL);
        pthread_join(th[1], NULL);

        // merging the two arrays
        merge(low, mid, high);
    }

    return NULL;
}

void merge(int low, int mid, int high)
{
    // creating left and right arrays
    int left_length = mid - low + 1;
    int right_length = high - mid;

    int left[left_length];
    int right[right_length];

    for (int i = 0; i < left_length; i++)
        left[i] = numbers[low + i];

    for (int i = 0; i < right_length; i++)
        right[i] = numbers[mid + i + 1];

    // merging the left and right arrays
    int i = 0, j = 0, k = low;

    while (i < left_length && j < right_length)
    {
        if (left[i] <= right[j])
        {
            numbers[k] = left[i];
            i++;
        }
        else
        {
            numbers[k] = right[j];
            j++;
        }
        k++;
    }

    // putting the remaining numbers after merging in the numbers array
    while (i < left_length)
    {
        numbers[k] = left[i];
        k++;
        i++;
    }

    while (j < right_length)
    {
        numbers[k] = right[j];
        k++;
        j++;
    }
}