// Definition des Knotens (Node)
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;            // Wert des Knotens (in diesem Fall ein Integer)
    struct Node* next;    // Zeiger auf den nächsten Knoten
} Node;

// Definition der dynamischen Liste (Linked List)
typedef struct {
    Node* head;  // Zeiger auf das erste Element der Liste
} LinkedList;

void init_list(LinkedList* list);

void insert_at_head(LinkedList* list, int value);

void print_list(const LinkedList* list);

void free_list(LinkedList* list);