#include <stdio.h>
#include <stdlib.h>

// Function to solve the 0/1 Knapsack Problem
int knapsack(int capacity, int weights[], int profits[], int n)
{
    int i, w;
    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    for (i = 0; i <= n; i++)
        dp[i] = (int *)malloc((capacity + 1) * sizeof(int));

    // Build the dp table
    for (i = 0; i <= n; i++)
    {
        for (w = 0; w <= capacity; w++)
        {
            if (i == 0 || w == 0)
                dp[i][w] = 0; // Base case: no items or zero capacity
            else if (weights[i - 1] <= w)
                dp[i][w] = (profits[i - 1] + dp[i - 1][w - weights[i - 1]] > dp[i - 1][w])
                               ? profits[i - 1] + dp[i - 1][w - weights[i - 1]]
                               : dp[i - 1][w];
            else
                dp[i][w] = dp[i - 1][w];
        }
    }

    // Store the result of the maximum profit
    int maxProfit = dp[n][capacity];

    // Print the selected items
    printf("Selected items (index starts from 1): ");
    w = capacity;
    for (i = n; i > 0 && maxProfit > 0; i--)
    {
        if (maxProfit != dp[i - 1][w])
        {
            printf("%d ", i); // Item i is included
            maxProfit -= profits[i - 1];
            w -= weights[i - 1];
        }
    }
    printf("\n");

    // Free allocated memory
    for (i = 0; i <= n; i++)
        free(dp[i]);
    free(dp);

    return dp[n][capacity];
}

int main()
{
    int n, capacity;

    // Input the number of items
    printf("Enter the number of items: ");
    scanf("%d", &n);

    // Input the weights and profits of the items
    int *weights = (int *)malloc(n * sizeof(int));
    int *profits = (int *)malloc(n * sizeof(int));
    printf("Enter the weights of the items: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &weights[i]);
    printf("Enter the profits of the items: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &profits[i]);

    // Input the capacity of the knapsack
    printf("Enter the capacity of the knapsack: ");
    scanf("%d", &capacity);

    // Solve the knapsack problem
    int maxProfit = knapsack(capacity, weights, profits, n);

    // Print the maximum profit
    printf("Maximum profit: %d\n", maxProfit);

    // Free allocated memory
    free(weights);
    free(profits);

    return 0;
}