#include <stdio.h>
#include <string.h>

// Compute the Longest Proper Prefix which is also Suffix (LPS) array
void computeLPS(const char *pat, int M, int *lps)
{
    int len = 0; // length of previous longest prefix suffix
    lps[0] = 0;  // lps[0] is always 0
    int i = 1;

    while (i < M)
    {
        if (pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            // poetic moment: “we grow our match one character at a time…”
            i++;
        }
        else
        {
            if (len != 0)
            {
                // we retreat len to try a shorter “prefix-suffix”—no need to advance i
                len = lps[len - 1];
            }
            else
            {
                // no match at all, lps[i] stays 0
                lps[i] = 0;
                i++;
            }
        }
    }
}

// KMP search: prints all starting indices where pat is found in txt
void KMPSearch(const char *pat, const char *txt)
{
    int M = strlen(pat);
    int N = strlen(txt);

    // allocate lps array
    int lps[M];
    computeLPS(pat, M, lps);

    int i = 0; // index for txt[]
    int j = 0; // index for pat[]

    printf("Pattern found at index(es): ");
    int found = 0;

    while (i < N)
    {
        if (pat[j] == txt[i])
        {
            i++;
            j++;
        }

        if (j == M)
        {
            // match ends at i-1, starts at i-j
            printf("%d ", i - j + 1);
            found = 1;
            // continue searching using lps to avoid re-scanning
            j = lps[j - 1];
        }
        else if (i < N && pat[j] != txt[i])
        {
            if (j != 0)
            {
                // jump j back to last known good border
                j = lps[j - 1];
            }
            else
            {
                // no border, just advance in text
                i++;
            }
        }
    }

    if (!found)
    {
        printf("none");
    }
    printf("\n");
}

int main()
{
    // Given Text and Pattern (space-separated for readability)
    char txt[] = "bacbababacababaca";
    char pat[] = "ababaca";

    printf("Text:    %s\n", txt);
    printf("Pattern: %s\n\n", pat);

    KMPSearch(pat, txt);
    return 0;
}
