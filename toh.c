#include <stdio.h>

int counter = 0;
void toh(int n, char a, char b, char c)
{
    if (n > 0)
    {
        counter++;
        toh(n - 1, a, c, b);
        printf("Disk %d moved from %c to %c\n", n, a, c);
        toh(n - 1, b, a, c);
    }
}

int main()
{
    int n;
    char A = 'S', B = 'T', C = 'T';
    printf("Enter number of discs:\n");
    scanf("%d", &n);
    toh(n, A, B, C);
    printf("%d", counter);
    return 0;
}