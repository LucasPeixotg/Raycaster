#include "linked_list.h"

#include <stdlib.h>

/**
 * Creates a new linked list and returns a pointer to it.
 * 
 * @return struct linked_list_of_lines* Pointer to the newly created linked list, or NULL if memory allocation fails.
 */
struct linked_list_of_lines* linked_list_create(void) {
    struct linked_list_of_lines* new = malloc(sizeof(struct linked_list_of_lines));
    if(new == NULL) return NULL;
    new->head = NULL;
    return new;
}

/**
 * Creates a new node with a given line value and pointer to the next node.
 * 
 * @param value The content (line) of the new node.
 * @param next A pointer to the next node in the list.
 * @return struct line_node* Pointer to the newly created node, or NULL if memory allocation fails.
 */
static struct line_node* create_line_node(struct line value, struct line_node* next) {
    struct line_node* new = malloc(sizeof(struct line_node));
    if(new == NULL) return NULL;
    new->value = value;
    new->next = next;
    return new;
}

/**
 * Adds a new line to the start of the linked list.
 * 
 * @param list The linked list to append the line to.
 * @param new_line The line to be added to the list.
 * @return int 0 if the insertion was successful, 1 otherwise.
 */
int linked_list_add_line(struct linked_list_of_lines* list, struct line new_line) {
    struct line_node* new_head = create_line_node(new_line, list->head);
    if(new_head == NULL) return 1;
    list->head = new_head;
    return 0;
}

/**
 * Recursively frees nodes in the linked list starting from the first node.
 * 
 * @param first The first node of the sequence to be deallocated.
 */
void destroy_list_recursive(struct line_node* first) {
    if(first == NULL) return;
    destroy_list_recursive(first->next);
    free(first);
}

/**
 * Destroys the linked list and frees all allocated resources.
 * 
 * @param list The linked list to be destroyed.
 */
void linked_list_destroy(struct linked_list_of_lines* list) {
    if(list == NULL) return;
    if(list->head != NULL) destroy_list_recursive(list->head);
    free(list);
}
