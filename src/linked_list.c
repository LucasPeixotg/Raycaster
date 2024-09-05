#include "linked_list.h"

#include <stdlib.h>

// Creates a new linked list and returns a pointer to it
struct linked_list_of_lines* linked_list_create(void) {
    struct linked_list_of_lines* new = malloc(sizeof(struct linked_list_of_lines));
    if(new == NULL) return NULL;
    new->head = NULL;
    return new;
}

/*
    Add a new line to the start of the linked list.
    Parameters:
        - struct line value: The content of the new node
        - struct line_node* next: The pointer to the next node
    Returns:
        - struct line_node*: pointer to the created node
*/
static struct line_node* create_line_node(struct line value, struct line_node* next) {
    struct line_node* new = malloc(sizeof(struct line_node));
    if(new == NULL) return NULL;
    new->value = value;
    new->next = next;
    return new;
}

/*
    Add a new line to the start of the linked list.
    Parameters:
        - struct linked_list_of_lines* list: The linked list to append the line to
        - struct line new_line: The line to be added to the list
    Returns:
        - int: 0 if the insertion was succesful, 1 otherwise
*/
int linked_list_add_line(struct linked_list_of_lines* list, struct line new_line) {
    struct line_node* new_head = create_line_node(new_line, list->head);
    if(new_head == NULL) return 1;
    list->head = new_head;
    return 0;
}

/*
    Static function to help destroy a linked list.
    Recursively freeing nodes
    Parameters:
        - struct line_node* first: First node of the sequence to be dealocated
*/
void destroy_list_recursive(struct line_node* first) {
    if(first == NULL) return;
    destroy_list_recursive(first->next);
    free(first);
}

/*
    Destroys the linked list and frees all allocated resources.
    Parameters:
        - struct linked_list_of_lines* list: The linked list to be destroyed
*/
void linked_list_destroy(struct linked_list_of_lines* list) {
    if(list == NULL) return;
    if(list->head != NULL) destroy_list_recursive(list->head);
    free(list);
}
