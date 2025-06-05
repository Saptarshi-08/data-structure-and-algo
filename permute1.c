#include <stdio.h>
#include <stdlib.h>

void swap(int *x, int *y)
{
    int t = *x;
    *x = *y;
    *y = t;
}

void heapPermute(int k, int *a, int n, FILE *f)
{
    if (k == 1)
    {
        for (int i = 0; i < n; i++)
            fprintf(f, "%d%c", a[i], i + 1 < n ? ' ' : '\n');
    }
    else
    {
        heapPermute(k - 1, a, n, f);
        for (int i = 0; i < k - 1; i++)
        {
            if (k % 2 == 0)
                swap(&a[i], &a[k - 1]);
            else
                swap(&a[0], &a[k - 1]);
            heapPermute(k - 1, a, n, f);
        }
    }
}

int main(void)
{
    int n;
    if (scanf("%d", &n) != 1 || n < 1)
        return 1;

    int *a = malloc(n * sizeof(int));
    if (!a)
        return 1;

    for (int i = 0; i < n; i++)
        a[i] = i + 1;

    FILE *f = fopen("perms.txt", "w");
    if (!f)
    {
        free(a);
        return 1;
    }

    heapPermute(n, a, n, f);

    fclose(f);
    free(a);
    return 0;
}
