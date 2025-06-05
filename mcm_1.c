#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct matrix
{
    int row;
    int col;
    int **data;
} matrix;

int matrixChainOrder(int *p, int n, int **m, int **s)
{
    for (int i = 1; i <= n; i++)
        m[i][i] = 0;

    for (int l = 2; l <= n; l++)
    {
        for (int i = 1; i <= n - l + 1; i++)
        {
            int j = i + l - 1;
            m[i][j] = INT_MAX;

            for (int k = i; k <= j - 1; k++)
            {
                int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j])
                {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }
    return m[1][n];
}

void printOptimalParens(int **s, int i, int j)
{
    if (i == j)
        printf("A%d", i);
    else
    {
        printf("(");
        printOptimalParens(s, i, s[i][j]);
        printOptimalParens(s, s[i][j] + 1, j);
        printf(")");
    }
}

matrix *allocateMatrix(int r, int c)
{
    matrix *m = (matrix *)malloc(sizeof(matrix));
    m->row = r;
    m->col = c;
    m->data = (int **)malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++)
        m->data[i] = (int *)malloc(c * sizeof(int));
    return m;
}

void freeMatrix(matrix *m)
{
    for (int i = 0; i < m->row; i++)
        free(m->data[i]);
    free(m->data);
    free(m);
}

void printMatrix(matrix *m)
{
    for (int i = 0; i < m->row; i++)
    {
        for (int j = 0; j < m->col; j++)
            printf("%d ", m->data[i][j]);
        printf("\n");
    }
}

matrix *copyMatrix(matrix *m)
{
    matrix *copy = allocateMatrix(m->row, m->col);
    for (int i = 0; i < m->row; i++)
        for (int j = 0; j < m->col; j++)
            copy->data[i][j] = m->data[i][j];
    return copy;
}

matrix *matrixMultiply(matrix *a, matrix *b)
{
    if (a->col != b->row)
    {
        printf("Invalid dimensions\n");
        return NULL;
    }

    matrix *c = allocateMatrix(a->row, b->col);
    for (int i = 0; i < a->row; i++)
    {
        for (int j = 0; j < b->col; j++)
        {
            c->data[i][j] = 0;
            for (int k = 0; k < a->col; k++)
                c->data[i][j] += a->data[i][k] * b->data[k][j];
        }
    }
    return c;
}

matrix *matrixChainMultiply(matrix **matrices, int **s, int i, int j)
{
    if (i == j)
        return copyMatrix(matrices[i]);
    else
    {
        matrix *a = matrixChainMultiply(matrices, s, i, s[i][j]);
        matrix *b = matrixChainMultiply(matrices, s, s[i][j] + 1, j);
        matrix *c = matrixMultiply(a, b);
        freeMatrix(a);
        freeMatrix(b);
        return c;
    }
}

int main()
{
    int n;
    printf("Enter the number of matrices: ");
    scanf("%d", &n);

    int *p = (int *)malloc((n + 1) * sizeof(int));
    printf("Enter the dimensions array (d0, d1, ..., d%d): ", n);
    for (int i = 0; i <= n; i++)
        scanf("%d", &p[i]);

    int **m = (int **)malloc((n + 1) * sizeof(int *));
    int **s = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++)
    {
        m[i] = (int *)malloc((n + 1) * sizeof(int));
        s[i] = (int *)malloc((n + 1) * sizeof(int));
    }

    int minCost = matrixChainOrder(p, n, m, s);
    printf("Minimum cost: %d\n", minCost);
    printOptimalParens(s, 1, n);
    printf("\n");

    matrix **matrices = (matrix **)malloc((n + 1) * sizeof(matrix *));
    srand(time(NULL));
    for (int i = 1; i <= n; i++)
    {
        int r = p[i - 1];
        int c = p[i];
        matrices[i] = allocateMatrix(r, c);
        for (int j = 0; j < r; j++)
            for (int k = 0; k < c; k++)
                matrices[i]->data[j][k] = (rand() % 9) + 1;
    }

    printf("Individual matrices:\n");
    for (int i = 1; i <= n; i++)
    {
        printf("Matrix A%d (%d x %d):\n", i, matrices[i]->row, matrices[i]->col);
        printMatrix(matrices[i]);
        printf("\n");
    }

    matrix *result = matrixChainMultiply(matrices, s, 1, n);
    printf("Resultant matrix:\n");
    printMatrix(result);

    for (int i = 1; i <= n; i++)
        freeMatrix(matrices[i]);
    free(matrices);
    freeMatrix(result);

    for (int i = 0; i <= n; i++)
    {
        free(m[i]);
        free(s[i]);
    }
    free(m);
    free(s);
    free(p);

    return 0;
}