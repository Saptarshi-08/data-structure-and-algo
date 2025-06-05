#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct
{
    int src, dest, weight;
} Edge;

typedef struct
{
    int *parent, *rank;
    int n;
} DisjointSet;

DisjointSet *makeSet(int n)
{
    // Create n singleton sets
    // Each element is its own parent, and rank is 0
    DisjointSet *ds = (DisjointSet *)malloc(sizeof(DisjointSet));

    ds->n = n;
    ds->parent = (int *)malloc(n * sizeof(int));
    ds->rank = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        ds->parent[i] = i;
        ds->rank[i] = 0;
    }
    return ds;
}

int findSet(DisjointSet *ds, int x)
{
    // find set with path compression
    if (ds->parent[x] != x)
    {
        ds->parent[x] = findSet(ds, ds->parent[x]);
    }
    return ds->parent[x];
}

void unionSet(DisjointSet *ds, int x, int y)
{
    // union of two sets by rank
    int rootX = findSet(ds, x);
    int rootY = findSet(ds, y);

    // If they are already in the same set
    if (rootX == rootY)
        return;

    if (ds->rank[rootX] < ds->rank[rootY])
    {
        ds->parent[rootX] = rootY;
    }
    else
    {
        ds->parent[rootY] = rootX;
        if (ds->rank[rootX] == ds->rank[rootY])
            ds->rank[rootX]++;
    }
}

// comparision function for qsort
int compare(const void *a, const void *b)
{
    const Edge *e1 = a;
    const Edge *e2 = b;
    return e1->weight - e2->weight;
}

int kruskalMST(int n, Edge edges[], int E)
{
    // n = number of vertices
    // E = number of edges
    qsort(edges, E, sizeof(Edge), compare);

    DisjointSet *ds = makeSet(n);

    int mstWeight = 0;
    int mstEdges = 0;

    printf("Edges in the Minimum Spanning Tree:\n");
    for (int i = 0; i < E && mstEdges < n - 1; i++)
    {
        int u = edges[i].src;
        int v = edges[i].dest;

        int rootU = findSet(ds, u);
        int rootV = findSet(ds, v);

        if (rootU != rootV)
        {
            // accept this edge in the mst
            printf("%d -- %d | Weight: %d\n", u, v, edges[i].weight);
            mstWeight += edges[i].weight;
            unionSet(ds, rootU, rootV);
            mstEdges++;
        }
    }
    if (mstEdges < n - 1)
    {
        printf("Graph is disconnected, no MST exists.\n");
    }

    free(ds->parent);
    free(ds->rank);
    free(ds);
    return mstWeight;
}

int main()
{
    FILE *fp = fopen("graph.txt", "r");
    if (!fp)
    {
        printf("Error opening file.\n");
        return 1;
    }
    int n = 0, E = 0;
    fscanf(fp, "%d", &n);

    Edge *edges = malloc(MAX * sizeof(Edge));
    while (E < MAX && fscanf(fp, "%d%d%d", &edges[E].src, &edges[E].dest, &edges[E].weight) == 3)
        E++;
    fclose(fp);

    if (!E)
    {
        printf("No edges found in the file.\n");
        free(edges);
        return 1;
    }

    int totalWeight = kruskalMST(n, edges, E);
    printf("Total weight of the Minimum Spanning Tree: %d\n", totalWeight);
    free(edges);
    return 0;
}