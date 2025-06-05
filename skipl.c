#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define PLUS_INFINITY INT_MAX
#define MINUS_INFINITY INT_MIN

typedef struct node
{
    int key;
    struct node *f;
    struct node *b;
    struct node *u;
    struct node *d;
} node;

typedef struct skiplist
{
    node *head;
    node *tail;
    int level; // number of levels in the skiplist
    int size;  // number of elements in the skiplist
} skiplist;

int toss()
{
    return rand() % 2; // Randomly returns 0 or 1
}

node *createNode(int key)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->key = key;
    newNode->f = NULL;
    newNode->b = NULL;
    newNode->u = NULL;
    newNode->d = NULL;
    return newNode;
}

skiplist *createSkiplist(skiplist *sl)
{

    node *p1 = createNode(MINUS_INFINITY);
    node *p2 = createNode(PLUS_INFINITY);

    p1->f = p2;
    p2->b = p1;
    sl->head = p1;
    sl->tail = p2;
    sl->level = 0;
    sl->size = 0;

    return sl;
}

skiplist *addEmptyList(skiplist *sl)
{
    node *p1 = createNode(MINUS_INFINITY);
    node *p2 = createNode(PLUS_INFINITY);

    p1->f = p2;
    p1->d = sl->head;

    p2->b = p1;
    p2->d = sl->tail;

    sl->head->u = p1;
    sl->tail->u = p2;

    sl->head = p1;
    sl->tail = p2;

    sl->level++;
    return sl;
}

node *search(skiplist *sl, int key)
{
    node *t = sl->head;
    while (1)
    {
        while (t->f->key <= key && t->f->key != PLUS_INFINITY)
        {
            t = t->f;
        }
        if (t->d != NULL)
            t = t->d;
        else
            break;
    }
    return t;
}

skiplist *insert(skiplist *sl, int value)
{
    node *p = search(sl, value);
    if (p->key == value)
    {
        printf("Value %d already exists in the skiplist.\n", value);
        return sl;
    }
    else
    {
        if (sl->head->f != sl->tail || sl->head->f->key != PLUS_INFINITY)
        {
            sl = addEmptyList(sl);
        }

        node *newNode = createNode(value);
        newNode->f = p->f;
        newNode->b = p;
        p->f->b = newNode;
        p->f = newNode;

        int current_level = 0;
        node *curr_node = newNode;
        while (toss())
        {
            if (current_level >= sl->level)
                sl = addEmptyList(sl);

            while (p->b && p->u == NULL)
                p = p->b;

            if (p->u == NULL)
                break;
            p = p->u;

            node *newNodeUp = createNode(value);
            newNodeUp->f = p->f;
            newNodeUp->b = p;
            newNodeUp->d = curr_node;
            p->f->b = newNodeUp;
            p->f = newNodeUp;
            curr_node->u = newNodeUp;
            curr_node = newNodeUp;

            current_level++;
            if (sl->head->f != sl->tail || sl->head->f->key != PLUS_INFINITY)
            {
                sl = addEmptyList(sl);
            }
        }
        sl->size++;
    }
    return sl;
}

skiplist *deleteValue(skiplist *sl, int value)
{
    node *p = search(sl, value);
    if (p->key != value)
    {
        printf("Value %d not found in the skiplist.\n", value);
        return sl;
    }

    // Delete all occurrences of value in all levels
    while (p != NULL)
    {
        node *temp = p;
        p->b->f = p->f; // Update forward pointer of previous node
        p->f->b = p->b; // Update backward pointer of next node
        p = p->u;       // Move up
        free(temp);     // Free current node
    }
    sl->size--;

    node *curr_lev = sl->head;
    while (curr_lev->d != NULL)
    {
        node *next_lev = curr_lev->d;
        if (curr_lev->f->key == PLUS_INFINITY && curr_lev->f == sl->tail)
        {
            node *curr_head = curr_lev;
            node *curr_tail = curr_lev->f;

            sl->head = sl->head->d;
            sl->tail = sl->tail->d;
            sl->level--;

            free(curr_head);
            free(curr_tail); // fixed the dangling pointer
        }
        curr_lev = next_lev; // Move to next level
    }

    if (sl->head->f != sl->tail || sl->head->f->key != PLUS_INFINITY)
    {
        sl = addEmptyList(sl);
    }
    return sl;
}

void display(skiplist *sl)
{
    node *t = sl->head;
    int lvl = sl->level;
    while (t)
    {
        node *q = t->f;
        printf("Level %d: -inf ", lvl--);
        while (q->key != PLUS_INFINITY)
        {
            printf("%d ", q->key);
            q = q->f;
        }
        printf(" +inf \n");
        t = t->d;
    }
}

void freeSkipList(skiplist *sl)
{
    node *curr_level = sl->head;
    while (curr_level)
    {
        node *n = curr_level;
        curr_level = curr_level->d;
        while (n)
        {
            node *temp = n;
            n = n->f;
            free(temp);
        }
    }
    free(sl);
    printf("Skiplist freed successfully.\n");
}

int main()
{
    srand(time(NULL)); // Seed for random number generation
    skiplist *sl = (skiplist *)malloc(sizeof(skiplist));
    sl = createSkiplist(sl);

    int choice, value;
    do
    {
        printf("1. Insert\n2. Delete\n3. Display\n4. Search\n5. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Enter value to insert: ");
            scanf("%d", &value);
            sl = insert(sl, value);
            break;
        case 2:
            printf("Enter value to delete: ");
            scanf("%d", &value);
            sl = deleteValue(sl, value);
            break;
        case 3:
            display(sl);
            break;
        case 4:
            printf("Enter a key to Search: ");
            scanf("%d", &value);
            node *p = search(sl, value);

            (p->key == value) ? printf("Key: %d FOUND\n", value) : printf("Key: %d NOT FOUND\n", value);
            break;
        case 5:
            freeSkipList(sl);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}