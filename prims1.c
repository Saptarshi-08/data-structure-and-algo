#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_VERTICES 100

typedef struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode *next;
} AdjListNode;

typedef struct
{
    AdjListNode *head;
} AdjList;

typedef struct
{
    int v;
    int key;
} HeapNode;

typedef struct
{
    int size;
    int capacity;
    int *pos;
    HeapNode **array;
} MinHeap;

// Create a new adjacency list node
AdjListNode *newAdjListNode(int dest, int weight)
{
    AdjListNode *node = (AdjListNode *)malloc(sizeof(AdjListNode));
    node->dest = dest;
    node->weight = weight;
    node->next = NULL;
    return node;
}

// Add undirected edge to adjacency list
void addEdge(AdjList adj[], int u, int v, int weight)
{
    AdjListNode *node = newAdjListNode(v, weight);
    node->next = adj[u].head;
    adj[u].head = node;

    node = newAdjListNode(u, weight);
    node->next = adj[v].head;
    adj[v].head = node;
}

HeapNode *newHeapNode(int v, int key)
{
    HeapNode *node = (HeapNode *)malloc(sizeof(HeapNode));
    node->v = v;
    node->key = key;
    return node;
}

MinHeap *createMinHeap(int capacity)
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->pos = (int *)malloc(capacity * sizeof(int));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (HeapNode **)malloc(capacity * sizeof(HeapNode *));
    return heap;
}

void swapHeapNode(HeapNode **a, HeapNode **b)
{
    HeapNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap *heap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->array[left]->key < heap->array[smallest]->key)
        smallest = left;

    if (right < heap->size && heap->array[right]->key < heap->array[smallest]->key)
        smallest = right;

    if (smallest != idx)
    {
        HeapNode *smallestNode = heap->array[smallest];
        HeapNode *idxNode = heap->array[idx];

        heap->pos[smallestNode->v] = idx;
        heap->pos[idxNode->v] = smallest;

        swapHeapNode(&heap->array[smallest], &heap->array[idx]);
        minHeapify(heap, smallest);
    }
}

int isEmpty(MinHeap *heap)
{
    return heap->size == 0;
}

HeapNode *extractMin(MinHeap *heap)
{
    if (isEmpty(heap))
        return NULL;

    HeapNode *root = heap->array[0];
    HeapNode *lastNode = heap->array[heap->size - 1];

    heap->array[0] = lastNode;
    heap->pos[root->v] = heap->size - 1;
    heap->pos[lastNode->v] = 0;

    --heap->size;
    minHeapify(heap, 0);

    return root;
}

void decreaseKey(MinHeap *heap, int v, int key)
{
    int i = heap->pos[v];
    heap->array[i]->key = key;

    while (i && heap->array[i]->key < heap->array[(i - 1) / 2]->key)
    {
        heap->pos[heap->array[i]->v] = (i - 1) / 2;
        heap->pos[heap->array[(i - 1) / 2]->v] = i;
        swapHeapNode(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInMinHeap(MinHeap *heap, int v)
{
    return heap->pos[v] < heap->size;
}

void primMST(AdjList adj[], int V)
{
    int parent[V];
    int key[V];
    int totalWeight = 0;

    MinHeap *heap = createMinHeap(V);

    // Choose a random source vertex
    srand(time(NULL));
    int src = rand() % V;

    for (int v = 0; v < V; ++v)
    {
        parent[v] = -1;
        key[v] = INT_MAX;
        heap->array[v] = newHeapNode(v, key[v]);
        heap->pos[v] = v;
    }

    key[src] = 0;
    decreaseKey(heap, src, key[src]);
    heap->size = V;

    while (!isEmpty(heap))
    {
        HeapNode *minNode = extractMin(heap);
        int u = minNode->v;

        AdjListNode *crawl = adj[u].head;
        while (crawl != NULL)
        {
            int v = crawl->dest;

            if (isInMinHeap(heap, v) && crawl->weight < key[v])
            {
                key[v] = crawl->weight;
                parent[v] = u;
                decreaseKey(heap, v, key[v]);
            }

            crawl = crawl->next;
        }
    }

    // Print MST
    printf("Starting from vertex: %d\n", src);
    printf("Edge\tWeight\n");
    for (int i = 0; i < V; ++i)
    {
        if (parent[i] != -1)
        {
            printf("%d - %d\t%d\n", parent[i], i, key[i]);
            totalWeight += key[i];
        }
    }

    printf("Total Weight of MST: %d\n", totalWeight);
}

int main()
{
    FILE *file = fopen("graph.txt", "r");
    if (file == NULL)
    {
        printf("Unable to open file.\n");
        return 1;
    }

    int V;
    fscanf(file, "%d", &V);

    AdjList adj[MAX_VERTICES];
    for (int i = 0; i < V; i++)
        adj[i].head = NULL;

    int u, v, w;
    while (fscanf(file, "%d %d %d", &u, &v, &w) == 3)
    {
        addEdge(adj, u, v, w);
    }

    fclose(file);
    primMST(adj, V);
    return 0;
}
