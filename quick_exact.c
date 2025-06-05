#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN 1
#define MAX 100
#define INPUT "input2.txt"
#define OUTPUT2 "output5.txt"

int counter_r = 0;

void swap(char *x, char *y)
{
    char temp = *x;
    *x = *y;
    *y = temp;
}

int factorial(int n)
{
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}

void permute(char *str, int l, int r, FILE *fp)
{
    if (l == r)
    {
        for (int i = 0; str[i] != '\0'; i++)
        {
            fprintf(fp, "%c ", str[i]);
        }
        fprintf(fp, "\n");
    }
    else
    {
        for (int i = l; i <= r; i++)
        {
            swap(&str[l], &str[i]);
            permute(str, l + 1, r, fp);
            swap(&str[l], &str[i]);
        }
    }
}

void read(int *A, int numElm, FILE *fp)
{
    int i = 0;
    while (i < numElm && fscanf(fp, "%d", &A[i]))
    {
        i++;
    }
}

void write(int *A, int numElm, FILE *fp)
{
    int i;
    for (i = 0; i < numElm; i++)
    {
        fprintf(fp, "%d\t", A[i]);
    }
    fprintf(fp, "\n");
}

int partition_random(int *A, int l, int h)
{
    int pivotIndex = l + rand() % (h - l);

    int temp = A[l];
    A[l] = A[pivotIndex];
    A[pivotIndex] = temp;

    int pivot = A[l];
    int i = l, j = h + 1;
    while (i < j)
    {
        do
        {
            i++;
            counter_r++;
        } while (i < h && A[i] <= pivot);
        do
        {
            j--;
            counter_r++;
        } while (j > l && A[j] >= pivot);
        if (i < j)
        {
            temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }
    }
    temp = A[l];
    A[l] = A[j];
    A[j] = temp;
    return j;
}

void quicksort_random(int *A, int l, int h)
{
    if (l < h)
    {
        int j = partition_random(A, l, h);
        quicksort_random(A, l, j);
        quicksort_random(A, j + 1, h);
    }
}

int main()
{
    int i, n;
    float total_counter_r = 0;
    printf("Enter the number of digits: ");
    scanf("%d", &n);

    int n_fac = factorial(n);

    int *A = (int *)calloc(n, sizeof(int));
    int *B = (int *)calloc(n, sizeof(int));

    char str[n + 1];
    for (int i = 0; i < n; i++)
    {
        str[i] = '1' + i;
    }
    str[n] = '\0';

    FILE *fp = fopen(INPUT, "w");
    if (fp == NULL)
    {
        printf("Error opening file for writing.\n");
        return 1;
    }

    permute(str, 0, n - 1, fp);
    fclose(fp);

    srand(time(NULL));
    FILE *fp1 = fopen(INPUT, "r");
    FILE *fp3 = fopen(OUTPUT2, "a+");

    for (int i = 0; i < n_fac; i++)
    {
        read(A, n, fp1);
        quicksort_random(A, 0, n - 1);
        printf("Number of inputs: %d | Number of comparisions: %d\n", n, counter_r);
        write(A, n, fp3);
        total_counter_r += counter_r;
        counter_r = 0;
    }

    printf("Number of inputs: %d | Number of comparisions: %f\n", n_fac * n, total_counter_r / n_fac);
    fclose(fp1);
    fclose(fp3);
    free(A);
    free(B);
    return 0;
}