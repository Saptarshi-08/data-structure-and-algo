#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Define the maximum level for the skip list
#define MAX_LEVEL 6

// Node structure for the skip list
typedef struct Node
{
    int key;
    struct Node **forward; // Array of forward pointers
} Node;

// Skip list structure
typedef struct SkipList
{
    int level;    // Current level of the skip list
    Node *header; // Header node
} SkipList;

// Function to create a new node
Node *createNode(int key, int level)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->forward = (Node **)malloc((level + 1) * sizeof(Node *));
    for (int i = 0; i <= level; i++)
    {
        node->forward[i] = NULL;
    }
    return node;
}

// Function to create a new skip list
SkipList *createSkipList()
{
    SkipList *list = (SkipList *)malloc(sizeof(SkipList));
    list->level = 0;
    list->header = createNode(INT_MIN, MAX_LEVEL); // Header node with minimum key
    return list;
}

// Function to generate a random level for a new node
int randomLevel()
{
    int level = 0;
    while (rand() % 2 && level < MAX_LEVEL)
    {
        level++;
    }
    return level;
}

// Function to insert a key into the skip list
void insert(SkipList *list, int key)
{
    Node *current = list->header;
    Node *update[MAX_LEVEL + 1];
    for (int i = list->level; i >= 0; i--)
    {
        while (current->forward[i] != NULL && current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current == NULL || current->key != key)
    {
        int newLevel = randomLevel();
        if (newLevel > list->level)
        {
            for (int i = list->level + 1; i <= newLevel; i++)
            {
                update[i] = list->header;
            }
            list->level = newLevel;
        }

        Node *newNode = createNode(key, newLevel);
        for (int i = 0; i <= newLevel; i++)
        {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
        printf("Inserted key %d\n", key);
    }
    else
    {
        printf("Key %d already exists in the skip list.\n", key);
    }
}

// Function to delete a key from the skip list
void delete(SkipList *list, int key)
{
    Node *current = list->header;
    Node *update[MAX_LEVEL + 1];
    for (int i = list->level; i >= 0; i--)
    {
        while (current->forward[i] != NULL && current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current != NULL && current->key == key)
    {
        for (int i = 0; i <= list->level; i++)
        {
            if (update[i]->forward[i] != current)
            {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }

        free(current);
        while (list->level > 0 && list->header->forward[list->level] == NULL)
        {
            list->level--;
        }
        printf("Deleted key %d\n", key);
    }
    else
    {
        printf("Key %d not found in the skip list.\n", key);
    }
}

// Function to search for a key in the skip list
Node *search(SkipList *list, int key)
{
    Node *current = list->header;
    for (int i = list->level; i >= 0; i--)
    {
        while (current->forward[i] != NULL && current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    if (current != NULL && current->key == key)
    {
        printf("Found key %d\n", key);
        return current;
    }
    printf("Key %d not found\n", key);
    return NULL;
}

// Function to traverse and print the skip list
void traverse(SkipList *list)
{
    printf("Skip List: ");
    Node *current = list->header->forward[0];
    while (current != NULL)
    {
        printf("%d ", current->key);
        current = current->forward[0];
    }
    printf("\n");
}

int main()
{
    SkipList *list = createSkipList();
    int choice, key;

    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Search\n");
        printf("4. Traverse\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter the key to insert: ");
            scanf("%d", &key);
            insert(list, key);
            break;
        case 2:
            printf("Enter the key to delete: ");
            scanf("%d", &key);
            delete (list, key);
            break;
        case 3:
            printf("Enter the key to search: ");
            scanf("%d", &key);
            search(list, key);
            break;
        case 4:
            traverse(list);
            break;
        case 5:
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}