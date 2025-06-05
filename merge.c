#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define INPUT_FILENAME "input.txt"
#define OUTPUT_FILENAME "output.txt"
#define MIN 1
#define MAX 10000
#define INPUTS 10

int counter = 0; // global variable to keep track of comparisions

void generate()
{
    int i;
    FILE *fileptr = fopen(INPUT_FILENAME, "w");
    if (fileptr == NULL)
    {
        printf("Error: Cannot open file\n");
        return;
    }

    srand(time(NULL));
    for (i = 0; i < INPUTS; i++)
    {
        int n = (rand() % MAX) + MIN;
        fprintf(fileptr, "%d\n", n);
    }
    fclose(fileptr);
}

void read(int *A)
{
    FILE *fileptr = fopen(INPUT_FILENAME, "r");
    if (fileptr == NULL)
    {
        printf("Error: Cannot open file\n");
        return;
    }

    int i = 0;
    while (fscanf(fileptr, "%d", &A[i]) == 1 && i < INPUTS)
        i++;

    fclose(fileptr);
}

void write(int *A)
{
    int i;
    FILE *fileptr = fopen(OUTPUT_FILENAME, "w");
    if (fileptr == NULL)
    {
        printf("Error: Cannot open file\n");
        return;
    }

    for (i = 0; i < INPUTS; i++)
    {
        fprintf(fileptr, "%d\n", A[i]);
    }

    fclose(fileptr);
}

void merge(int *A, int l, int mid, int h)
{
    int i, j, k;
    int n1 = mid - l + 1;
    int n2 = h - mid;

    int *L = (int *)calloc(n1, sizeof(int));
    int *R = (int *)calloc(n2, sizeof(int));

    for (i = 0; i < n1; i++) // debug
        L[i] = A[l + i];
    for (j = 0; j < n2; j++) // debug
        R[j] = A[mid + 1 + j];

    i = j = 0;
    k = l;

    while (i < n1 && j < n2) // debug
    {
        counter++;
        if (L[i] <= R[j]) // debug
        {
            A[k] = L[i];
            i++;
        }
        else
        {
            A[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of L[]
    while (i < n1) // debug
    {
        A[k] = L[i];
        i++;
        k++;
    }

    // Copy remaining elements of R[]
    while (j < n2) // debug
    {
        A[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergesort(int *A, int l, int h)
{
    if (l < h)
    {
        int mid = (l + h) / 2;
        mergesort(A, l, mid);
        mergesort(A, mid + 1, h);
        merge(A, l, mid, h);
    }
}

int main()
{
    int i;
    int *A = (int *)calloc(INPUTS, sizeof(int));
    generate();
    read(A);
    mergesort(A, 0, INPUTS - 1);
    printf("No of Inputs: %d | No. of Comparisions: %d\n", INPUTS, counter);
    write(A);
    free(A);

    return 0;
}