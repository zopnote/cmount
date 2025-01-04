#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// Funktion zur Initialisierung der Liste
void init_list(LinkedList* list) {
    list->head = NULL;
}

// Funktion zum Einfügen eines neuen Knotens am Kopf der Liste
void insert_at_head(LinkedList* list, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));  // Speicher für den neuen Knoten allozieren
    if (new_node == NULL) {
        printf("Fehler: Speicher konnte nicht zugewiesen werden!\n");
        return;
    }

    new_node->value = value;  // Wert zuweisen
    new_node->next = list->head;  // Zeiger auf den bisherigen Kopf setzen
    list->head = new_node;  // Der neue Knoten wird zum Kopf der Liste
}

// Funktion zum Ausgeben der Liste
void print_list(const LinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        printf("%d -> ", current->value);
        current = current->next;
    }
    printf("NULL\n");  // Ende der Liste anzeigen
}

// Funktion zum Freigeben des Speichers der Liste
void free_list(LinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);  // Speicher des aktuellen Knotens freigeben
    }
    list->head = NULL;  // Kopf der Liste auf NULL setzen
    free(list);
}