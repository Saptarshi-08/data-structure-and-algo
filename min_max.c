#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int comparisions = 0;

void generate(int elm)
{
    FILE *fp = fopen("input3.txt", "w");
    srand(time(NULL));
    for (int i = 0; i < elm; i++)
    {
        int n = (rand() % 100) + 1;
        fprintf(fp, "%d\t", n);
    }
    fclose(fp);
}

void read(int *A, int elm, FILE *fp)
{
    int i = 0;
    while (i < elm && fscanf(fp, "%d", &A[i]))
    {
        i++;
    }
}

void odd_n(int *A, int elm)
{
    int max = A[0], min = A[0];
    for (int i = 1; i < elm; i = i + 2)
    {
        comparisions++;
        if (A[i] > A[i + 1])
        {
            comparisions++;
            if (A[i] > max)
            {

                max = A[i];
            }
            comparisions++;
            if (A[i + 1] < min)
            {

                min = A[i + 1];
            }
        }
        else
        {
            comparisions++;
            if (A[i + 1] > max)
            {

                max = A[i + 1];
            }
            comparisions++;
            if (A[i] < min)
            {

                min = A[i];
            }
        }
    }

    printf("Max: %d | Min: %d | Comparisions: %d\n", max, min, comparisions);
}

void even_n(int *A, int elm)
{
    int max = 0, min = 0;
    if (A[0] > A[1])
    {
        comparisions++;
        max = A[0];
        min = A[1];
    }
    else
    {
        comparisions++;
        max = A[1];
        min = A[0];
    }
    for (int i = 2; i < elm; i = i + 2)
    {
        comparisions++;
        if (A[i] > A[i + 1])
        {
            comparisions++;
            if (A[i] > max)
            {

                max = A[i];
            }
            comparisions++;
            if (A[i + 1] < min)
            {

                min = A[i + 1];
            }
        }
        else
        {
            comparisions++;
            if (A[i + 1] > max)
            {

                max = A[i + 1];
            }
            comparisions++;
            if (A[i] < min)
            {

                min = A[i];
            }
        }
    }

    printf("Max: %d | Min: %d | Comparisions: %d\n", max, min, comparisions);
}

int main()
{
    int elm;
    printf("Enter the number of elements:\n");
    scanf("%d", &elm);
    generate(elm);
    FILE *fp = fopen("input3.txt", "r");
    int *A = (int *)calloc(elm, sizeof(int));
    read(A, elm, fp);
    if (elm % 2 == 0)
    {
        even_n(A, elm);
    }
    else
    {
        odd_n(A, elm);
    }
    fclose(fp);
    return 0;
}