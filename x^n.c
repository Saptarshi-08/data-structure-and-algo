#include <stdio.h>

long long int linear_Xn(int X, int n)
{
    static int counter = 0;
    if (n == 0)
    {
        counter++;
        return 1;
    }
    else
    {
        long long int result = X * linear_Xn(X, n - 1);
        printf("Step: %d | n = %d result = %lld\n", counter++, n, result);
        return result;
    }
}

long long int log_Xn(int X, int n)
{
    static int counter = 0;
    if (n == 1)
        return X;
    long long int halfpower = log_Xn(X, n / 2);
    if (n % 2 == 0)
    {
        counter++;
        long long int result = halfpower * halfpower;
        printf("Step: %d | halfpower = %d result = %lld\n", counter, halfpower, result);
        return result;
    }
    else
    {
        counter++;
        long long int result = X * halfpower * halfpower;
        printf("Step: %d | halfpower = %d result = %lld\n", counter, halfpower, result);
        return result;
    }
}

int main()
{
    int n, X;
    printf("Enter the values of X and n: ");
    scanf("%d %d", &X, &n);
    printf("For Linear Time X^n:\n");
    linear_Xn(X, n);
    printf("\nFor Log Time X^n:\n");
    log_Xn(X, n);

    return 0;
}
