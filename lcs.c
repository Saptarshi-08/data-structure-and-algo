#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to find the length of the LCS and build the LCS table
int lcs(char *X, char *Y, int m, int n, int **dp)
{
    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (i == 0 || j == 0)
                dp[i][j] = 0; // Base case: LCS of empty string is 0
            else if (X[i - 1] == Y[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1; // Characters match
            else
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1]) ? dp[i - 1][j] : dp[i][j - 1]; // Take the maximum
        }
    }

    return dp[m][n]; // Length of LCS
}

// Function to print the LCS using the dp table
void printLCS(char *X, char *Y, int m, int n, int **dp)
{
    int index = dp[m][n];
    char *lcsStr = (char *)malloc((index + 1) * sizeof(char));
    lcsStr[index] = '\0'; // Null-terminate the LCS string

    int i = m, j = n;
    while (i > 0 && j > 0)
    {
        if (X[i - 1] == Y[j - 1])
        {
            lcsStr[index - 1] = X[i - 1]; // Add character to LCS
            i--;
            j--;
            index--;
        }
        else if (dp[i - 1][j] > dp[i][j - 1])
            i--; // Move up in the table
        else
            j--; // Move left in the table
    }

    printf("Longest Common Subsequence: %s\n", lcsStr);
    free(lcsStr);
}

int main()
{
    char X[100], Y[100];

    // Input the two strings
    printf("Enter the first string: ");
    scanf("%s", X);
    printf("Enter the second string: ");
    scanf("%s", Y);

    int m = strlen(X);
    int n = strlen(Y);

    // Allocate memory for the dp table
    int **dp = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++)
        dp[i] = (int *)malloc((n + 1) * sizeof(int));

    // Find the length of LCS
    int lcsLength = lcs(X, Y, m, n, dp);

    // Print the length of LCS
    printf("Length of Longest Common Subsequence: %d\n", lcsLength);

    // Print the LCS
    printLCS(X, Y, m, n, dp);

    // Free allocated memory
    for (int i = 0; i <= m; i++)
        free(dp[i]);
    free(dp);

    return 0;
}