#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int LCS(char *a, char *b, int i, int j, int **dp)
{
    if (i < 0 || j < 0)
        return 0;
    if (dp[i][j] != -1)
        return dp[i][j];

    if (a[i] == b[j])
        dp[i][j] = 1 + LCS(a, b, i - 1, j - 1, dp);
    else
    {
        int left = LCS(a, b, i - 1, j, dp);
        int up = LCS(a, b, i, j - 1, dp);
        dp[i][j] = (left > up) ? left : up;
    }
    return dp[i][j];
}

void printLCS(char *a, char *b, int i, int j, int **dp)
{
    if (i < 0 || j < 0)
        return;

    if (a[i] == b[j])
    {
        printLCS(a, b, i - 1, j - 1, dp);
        printf("%c", a[i]);
    }
    else
    {
        if (i == 0 && j > 0)
        {
            printLCS(a, b, i, j - 1, dp);
        }
        else if (j == 0 && i > 0)
        {
            printLCS(a, b, i - 1, j, dp);
        }
        else
        {
            if (dp[i - 1][j] >= dp[i][j - 1])
                printLCS(a, b, i - 1, j, dp);
            else
                printLCS(a, b, i, j - 1, dp);
        }
    }
}

int main()
{
    char A[100], B[100];
    printf("Enter the first string: ");
    scanf("%s", A);
    printf("Enter the second string: ");
    scanf("%s", B);

    int m = strlen(A), n = strlen(B);

    int **dp = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++)
    {
        dp[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
            dp[i][j] = -1;
    }

    int lcsLen = LCS(A, B, m - 1, n - 1, dp);
    printf("Length of LCS: %d\n", lcsLen);

    printf("LCS: ");
    printLCS(A, B, m - 1, n - 1, dp);
    printf("\n");

    for (int i = 0; i < m; i++)
        free(dp[i]);
    free(dp);

    return 0;
}
