#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT "input1.txt"
#define OUTPUT "output1.txt"
#define MAX 100
#define MIN 1

int counter = 0;

void generate(int numArr, int numElm)
{
    FILE *fp = fopen(INPUT, "w");
    srand(time(NULL));
    for (int j = 0; j < numArr; j++)
    {
        for (int i = 0; i < numElm; i++)
        {
            int n = (rand() % MAX) + MIN;
            fprintf(fp, "%d\t", n);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
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

void merge(int *A, int l, int mid, int h)
{
    int i, j, k;
    int n1 = mid - l + 1;
    int n2 = h - mid;
    int *L = (int *)calloc(n1, sizeof(int));
    int *R = (int *)calloc(n2, sizeof(int));

    for (i = 0; i < n1; i++)
    {
        L[i] = A[l + i];
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = A[mid + 1 + j];
    }
    i = j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        counter++;
        if (L[i] <= R[j])
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
    while (i < n1)
    {
        A[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        A[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int *A, int l, int h)
{
    if (l < h)
    {
        int mid = (l + h) / 2;
        mergeSort(A, l, mid);
        mergeSort(A, mid + 1, h);
        merge(A, l, mid, h);
    }
}

int main()
{
    int i;
    float total_counter = 0;
    int numArr = 0, numElm = 0;
    printf("Enter the number of arrays and elements:\n");
    scanf("%d%d", &numArr, &numElm);
    int *A = (int *)calloc(numElm, sizeof(int));
    generate(numArr, numElm);
    FILE *fp1 = fopen(INPUT, "r");
    FILE *fp2 = fopen(OUTPUT, "a+");
    for (int i = 0; i < numArr; i++)
    {
        read(A, numElm, fp1);
        mergeSort(A, 0, numElm - 1);
        printf("Number of inputs: %d | Number of comparisions: %d\n", numElm, counter);
        write(A, numElm, fp2);
        total_counter += counter;
        counter = 0;
    }
    printf("Number of inputs: %d | Number of comparisions: %f\n", numArr * numElm, total_counter / numArr);
    fclose(fp1);
    fclose(fp2);
    free(A);
    return 0;
}
