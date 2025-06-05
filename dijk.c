#include <stdio.h>
#include <stdlib.h>

#define INF 9999 // A value to represent "infinity" (no edge)

// Structure for a node in the adjacency list
typedef struct Node
{
    int vertex;
    struct Node *next;
} Node;

// Structure for the graph (using an array of adjacency lists)
typedef struct Graph
{
    int numVertices;
    Node **adjLists;
} Graph;

// Create a new node
Node *createNode(int vertex)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

// Create a graph of given vertices
Graph *createGraph(int vertices)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjLists = (Node **)malloc(vertices * sizeof(Node *));
    for (int i = 0; i < vertices; i++)
    {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

// Add an edge from src to dest in the adjacency list
void addEdge(Graph *graph, int src, int dest)
{
    Node *newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

// Print the adjacency list representation of graph
void printGraph(Graph *graph)
{
    for (int v = 0; v < graph->numVertices; v++)
    {
        Node *temp = graph->adjLists[v];
        printf("Vertex %d: ", v);
        while (temp)
        {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

// Create and fill the adjacency matrix from the adjacency list
void createAdjMatrix(Graph *graph, int **matrix)
{
    int V = graph->numVertices;
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            if (i == j)
                matrix[i][j] = 0;
            else
                matrix[i][j] = INF;
        }
    }
    for (int i = 0; i < V; i++)
    {
        Node *temp = graph->adjLists[i];
        while (temp)
        {
            int j = temp->vertex;
            printf("Enter weight for edge from %d to %d: ", i + 1, j + 1); // Adjust for 1-based indexing
            scanf("%d", &matrix[i][j]);
            temp = temp->next;
        }
    }
}

// Implementation of Dijkstra's algorithm
void dijkstra(int **matrix, int vertices, int src, int dist[], int pred[])
{
    int visited[vertices];
    for (int i = 0; i < vertices; i++)
    {
        dist[i] = INF;
        visited[i] = 0;
        pred[i] = -1;
    }
    dist[src] = 0;

    for (int count = 0; count < vertices - 1; count++)
    {
        int min = INF, min_index;
        for (int v = 0; v < vertices; v++)
        {
            if (!visited[v] && dist[v] <= min)
            {
                min = dist[v];
                min_index = v;
            }
        }
        int u = min_index;
        visited[u] = 1;

        for (int v = 0; v < vertices; v++)
        {
            if (!visited[v] && matrix[u][v] != INF && dist[u] != INF &&
                dist[u] + matrix[u][v] < dist[v])
            {
                dist[v] = dist[u] + matrix[u][v];
                pred[v] = u;
            }
        }
    }
}

// Recursive function to print the path from source to vertex 'j'
void printPath(int pred[], int j)
{
    if (pred[j] == -1)
    {
        printf("%d", j + 1); // Adjust for 1-based indexing
        return;
    }
    printPath(pred, pred[j]);
    printf(" -> %d", j + 1); // Adjust for 1-based indexing
}

int main()
{
    int vertices, edges;
    printf("Enter the number of vertices: ");
    scanf("%d", &vertices);
    printf("Enter the number of edges: ");
    scanf("%d", &edges);

    Graph *graph = createGraph(vertices);
    for (int i = 0; i < edges; i++)
    {
        int src, dest;
        printf("Enter edge %d (source destination): ", i + 1);
        scanf("%d %d", &src, &dest);
        addEdge(graph, src - 1, dest - 1); // Convert to 0-based indexing
    }

    printf("\nAdjacency List of the Graph:\n");
    printGraph(graph);

    int **matrix = (int **)malloc(vertices * sizeof(int *));
    for (int i = 0; i < vertices; i++)
    {
        matrix[i] = (int *)malloc(vertices * sizeof(int));
    }

    createAdjMatrix(graph, matrix);

    printf("\nAdjacency Matrix:\n");
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (matrix[i][j] == INF)
                printf("%4s", "INF");
            else
                printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }

    int src;
    printf("\nEnter source vertex: ");
    scanf("%d", &src);
    src -= 1; // Convert to 0-based indexing

    if (src < 0 || src >= vertices)
    {
        printf("Invalid source vertex.\n");
        return 1;
    }

    int *dist = (int *)malloc(vertices * sizeof(int));
    int *pred = (int *)malloc(vertices * sizeof(int));

    dijkstra(matrix, vertices, src, dist, pred);

    printf("\nVertex   Distance from Source\n");
    for (int i = 0; i < vertices; i++)
    {
        printf("%4d \t\t", i + 1); // Adjust for 1-based indexing
        if (dist[i] == INF)
            printf("INF\n");
        else
            printf("%4d\n", dist[i]);
    }

    printf("\nShortest paths from vertex %d:\n", src + 1); // Adjust for 1-based indexing
    for (int i = 0; i < vertices; i++)
    {
        if (i != src)
        {
            printf("Path to vertex %d: ", i + 1); // Adjust for 1-based indexing
            if (dist[i] == INF)
                printf("No path\n");
            else
            {
                printPath(pred, i);
                printf("\n");
            }
        }
    }

    for (int i = 0; i < vertices; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    free(dist);
    free(pred);

    for (int i = 0; i < vertices; i++)
    {
        Node *temp = graph->adjLists[i];
        while (temp)
        {
            Node *next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(graph->adjLists);
    free(graph);

    return 0;
}
