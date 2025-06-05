#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 5

typedef struct Node
{
    int vertex;
    int weight;
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
} AdjList;

typedef struct
{
    int vertex;
    int distance;
} HeapNode;

typedef struct
{
    HeapNode *nodes;
    int size;
    int *pos;
} MinHeap;

Node *createNode(int vertex, int weight)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

void addEdge(AdjList *graph, int src, int dest, int weight)
{
    Node *newNode = createNode(dest, weight);
    newNode->next = graph[src].head;
    graph[src].head = newNode;

    newNode = createNode(src, weight);
    newNode->next = graph[dest].head;
    graph[dest].head = newNode;
}

MinHeap *createMinHeap(int capacity)
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->nodes = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    heap->pos = (int *)malloc(capacity * sizeof(int));
    heap->size = 0;
    return heap;
}

void swapHeapNodes(HeapNode *a, HeapNode *b)
{
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap *heap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance)
        smallest = left;
    if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance)
        smallest = right;

    if (smallest != idx)
    {
        HeapNode smallestNode = heap->nodes[smallest];
        HeapNode idxNode = heap->nodes[idx];

        heap->pos[smallestNode.vertex] = idx;
        heap->pos[idxNode.vertex] = smallest;

        swapHeapNodes(&heap->nodes[smallest], &heap->nodes[idx]);
        minHeapify(heap, smallest);
    }
}

int isEmpty(MinHeap *heap)
{
    return heap->size == 0;
}

HeapNode extractMin(MinHeap *heap)
{
    HeapNode root = heap->nodes[0];
    HeapNode lastNode = heap->nodes[heap->size - 1];
    heap->nodes[0] = lastNode;

    heap->pos[root.vertex] = heap->size - 1;
    heap->pos[lastNode.vertex] = 0;

    heap->size--;
    minHeapify(heap, 0);

    return root;
}

void decreaseKey(MinHeap *heap, int v, int dist)
{
    int i = heap->pos[v];
    heap->nodes[i].distance = dist;

    while (i && heap->nodes[i].distance < heap->nodes[(i - 1) / 2].distance)
    {
        heap->pos[heap->nodes[i].vertex] = (i - 1) / 2;
        heap->pos[heap->nodes[(i - 1) / 2].vertex] = i;
        swapHeapNodes(&heap->nodes[i], &heap->nodes[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInMinHeap(MinHeap *heap, int v)
{
    return heap->pos[v] < heap->size;
}

void printPath(int parent[], int j)
{
    if (parent[j] == -1)
    {
        printf("%c", 'S' + j);
        return;
    }
    printPath(parent, parent[j]);
    printf(" -> %c", 'S' + j);
}

void dijkstra(AdjList *graph, int src)
{
    int dist[MAX_VERTICES];
    int parent[MAX_VERTICES];

    MinHeap *heap = createMinHeap(MAX_VERTICES);

    for (int v = 0; v < MAX_VERTICES; v++)
    {
        dist[v] = INT_MAX;
        heap->nodes[v].vertex = v;
        heap->nodes[v].distance = INT_MAX;
        heap->pos[v] = v;
        parent[v] = -1;
    }

    dist[src] = 0;
    heap->nodes[src].distance = 0;
    heap->size = MAX_VERTICES;
    decreaseKey(heap, src, 0); // Use decreaseKey to move src up

    while (!isEmpty(heap))
    {
        HeapNode minNode = extractMin(heap);
        int u = minNode.vertex;

        Node *temp = graph[u].head;
        while (temp)
        {
            int v = temp->vertex;
            if (isInMinHeap(heap, v) && dist[u] != INT_MAX && temp->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + temp->weight;
                parent[v] = u;
                decreaseKey(heap, v, dist[v]);
            }
            temp = temp->next;
        }
    }

    printf("Vertex  Distance        Path\n");
    for (int i = 0; i < MAX_VERTICES; i++)
    {
        printf("%c       %d               ", 'S' + i, dist[i]);
        printPath(parent, i);
        printf("\n");
    }

    free(heap->nodes);
    free(heap->pos);
    free(heap);
}

// void dijkstra(AdjList* graph, int src, int V) {
//     int* dist = (int*)malloc(V * sizeof(int));
//     int* parent = (int*)malloc(V * sizeof(int));
//     MinHeap* heap = createMinHeap(V);

//     for (int v = 0; v < V; v++) {
//         dist[v] = INT_MAX;
//         heap->nodes[v].vertex = v;
//         heap->nodes[v].distance = dist[v];
//         heap->pos[v] = v;
//         parent[v] = -1;
//     }

//     dist[src] = 0;
//     heap->nodes[src].distance = 0;
//     heap->size = V;
//     decreaseKey(heap, src, 0);

//     while (!isEmpty(heap)) {
//         HeapNode minNode = extractMin(heap);
//         int u = minNode.vertex;

//         Node* temp = graph[u].head;
//         while (temp) {
//             int v = temp->vertex;
//             if (isInMinHeap(heap, v) && dist[u] != INT_MAX && temp->weight + dist[u] < dist[v]) {
//                 dist[v] = dist[u] + temp->weight;
//                 parent[v] = u;
//                 decreaseKey(heap, v, dist[v]);
//             }
//             temp = temp->next;
//         }
//     }

//     printf("Vertex  Distance        Path\n");
//     for (int i = 0; i < V; i++) {
//         printf("%c       %d               ", 'S' + i, dist[i]);
//         printPath(parent, i);
//         printf("\n");
//     }

//     free(dist);
//     free(parent);
// }

int main()
{
    AdjList graph[MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++)
    {
        graph[i].head = NULL;
    }

    addEdge(graph, 0, 3, 5); // S - V
    addEdge(graph, 3, 1, 3); // V - T
    addEdge(graph, 1, 2, 1); // T - U
    addEdge(graph, 3, 4, 2); // V - W

    dijkstra(graph, 0); // Start from S (0)

    return 0;
}

// int main() {
//     FILE *file = fopen("graph.txt", "r");
//     if (!file) {
//         perror("Error opening file");
//         return 1;
//     }

//     int V;
//     fscanf(file, "%d", &V); // Read number of vertices

//     AdjList* graph = (AdjList*)malloc(V * sizeof(AdjList));
//     for (int i = 0; i < V; i++) {
//         graph[i].head = NULL;
//     }

//     int src, dest, weight;
//     while (fscanf(file, "%d %d %d", &src, &dest, &weight) == 3) {
//         addEdge(graph, src, dest, weight);
//     }

//     fclose(file);

//     // Choose a random source vertex
//     srand(time(NULL));
//     int source = rand() % V;

//     printf("Running Dijkstra's Algorithm from source vertex: %d\n\n", source);
//     dijkstra(graph, source, V);

//     free(graph);
//     return 0;
// }
