#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

// Function to recursively calculate the minimum cost of matrix chain multiplication
int matrixChainOrder(int *p, int n, int **m, int **k)
{
    for (int i = 1; i < n; i++)
        m[i][i] = 0; // Cost of multiplying one matrix is 0

    for (int chainLength = 2; chainLength < n; chainLength++)
    {
        for (int i = 1; i < n - chainLength + 1; i++)
        {
            int j = i + chainLength - 1;
            m[i][j] = INT_MAX;

            for (int split = i; split < j; split++)
            {
                int cost = m[i][split] + m[split + 1][j] + p[i - 1] * p[split] * p[j];
                if (cost < m[i][j])
                {
                    m[i][j] = cost;
                    k[i][j] = split; // Store the split point
                }
            }
        }
    }

    return m[1][n - 1]; // Return the minimum cost
}

// Function to print the optimal parenthesization
void printOptimalParenthesis(int **k, int i, int j)
{
    if (i == j)
    {
        printf("A%d", i);
        return;
    }
    printf("(");
    printOptimalParenthesis(k, i, k[i][j]);
    printOptimalParenthesis(k, k[i][j] + 1, j);
    printf(")");
}

int main()
{
    int n;

    // Input the number of matrices
    printf("Enter the number of matrices: ");
    scanf("%d", &n);

    // Input the dimensions array
    int *p = (int *)malloc((n + 1) * sizeof(int));
    printf("Enter the dimensions array (d0, d1, ..., d%d): ", n);
    for (int i = 0; i <= n; i++)
    {
        scanf("%d", &p[i]);
    }

    // Allocate memory for m and k tables
    int **m = (int **)malloc((n + 1) * sizeof(int *));
    int **k = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++)
    {
        m[i] = (int *)malloc((n + 1) * sizeof(int));
        k[i] = (int *)malloc((n + 1) * sizeof(int));
    }

    // Calculate the minimum cost
    int minCost = matrixChainOrder(p, n + 1, m, k);

    // Print the results
    printf("Minimum number of scalar multiplications: %d\n", minCost);
    printf("Optimal parenthesization: ");
    printOptimalParenthesis(k, 1, n);
    printf("\n");

    // Free allocated memory
    for (int i = 0; i <= n; i++)
    {
        free(m[i]);
        free(k[i]);
    }
    free(m);
    free(k);
    free(p);

    return 0;
}