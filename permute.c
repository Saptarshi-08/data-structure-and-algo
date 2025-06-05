#include <stdio.h>

// Function to swap two characters
void swap(char *x, char *y)
{
    char temp = *x;
    *x = *y;
    *y = temp;
}

// Recursive function to generate permutations and write them to the file
void permute(char *str, int l, int r, FILE *fp)
{
    if (l == r)
    {
        // Print each character space separated
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
            swap(&str[l], &str[i]); // backtrack
        }
    }
}

int main()
{
    int n;
    printf("Enter the number of digits: ");
    scanf("%d", &n);

    // Create an array to hold the digits as characters plus the null terminator
    char str[n + 1];
    for (int i = 0; i < n; i++)
    {
        str[i] = '1' + i; // fills with '1', '2', ... up to n digits
    }
    str[n] = '\0'; // null terminate the string

    // Open the output file for writing
    FILE *fp = fopen("output4.txt", "w");
    if (fp == NULL)
    {
        printf("Error opening file for writing.\n");
        return 1;
    }

    // Generate permutations and write them to the file
    permute(str, 0, n - 1, fp);

    fclose(fp);
    printf("All permutations have been written to output.txt\n");

    return 0;
}
