#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int vertex;
    struct node *next;
} node;

typedef struct Graph
{
    int numVertices;
    struct node **adjLists;
} Graph;

node *createNode(int v)
{
    node *newnode = malloc(sizeof(node));
    newnode->vertex = v;
    newnode->next = NULL;
    return newnode;
}

Graph *createGraph(int vertices)
{
    Graph *graph = malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(node *));
    for (int i = 0; i < vertices; i++)
    {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

void addEdge(Graph *graph, int src, int dest)
{
    // Add edge from src to dest
    node *newnode = createNode(dest);
    newnode->next = graph->adjLists[src];
    graph->adjLists[src] = newnode;

    // Add edge from dest to src
    newnode = createNode(src);
    newnode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newnode;
}

void printGraph(Graph *graph)
{
    for (int v = 0; v < graph->numVertices; v++)
    {
        node *temp = graph->adjLists[v];
        printf("\n Adjacency list of vertex %d\n ", v);
        while (temp)
        {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

int main()
{
    int v;
    printf("Enter the number of vertices: ");
    scanf("%d", &v);
    Graph *graph = createGraph(v);
    while (1)
    {
        int src, dest;
        printf("Enter the source and destination vertices: ");
        scanf("%d %d", &src, &dest);
        if (src == -1 && dest == -1)
            break;
        addEdge(graph, src, dest);
    }
    printGraph(graph);
    return 0;
}