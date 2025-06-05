#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
    int vertex;
    int weight;
    struct node *next;
} Node;

typedef struct
{
    Node *head;
} AdjList;

typedef struct
{
    int V;
    AdjList *arr;
} Graph;

Node *createNode(int v, int w)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->weight = w;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int v)
{
    Graph *g = (Graph *)malloc(v * sizeof(Graph));
    g->V = v;
    g->arr = (AdjList *)malloc(v * sizeof(AdjList));
    for (int i = 0; i < v; i++)
    {
        g->arr[i].head = NULL;
    }
    return g;
}

void addEdge(Graph *g, int s, int d, int w)
{
    Node *newNode = createNode(d, w);
    newNode->next = g->arr[s].head;
    g->arr[s].head = newNode;
}

void printGraph(Graph *g)
{
    printf("Adjacency List Representation of Graph:\n");
    for (int i = 0; i < g->V; i++)
    {
        printf("Vertex %d: ", i);
        Node *curr = g->arr[i].head;
        while (curr)
        {
            printf(" -> %d (weight: %d) ", curr->vertex, curr->weight);
            curr = curr->next;
        }
        printf("\n");
    }
}

int main()
{
    FILE *fp = fopen("graph1.txt", "r");
    int V;
    fscanf(fp, "%d", &V);
    Graph *g = createGraph(V);
    int s, d, w;
    while (fscanf(fp, "%d%d%d", &s, &d, &w) == 3)
    {
        addEdge(g, s, d, w);
    }
    fclose(fp);
    printGraph(g);
    free(g->arr);
    free(g);

    return 0;
}