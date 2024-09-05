#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "algebra.h"

// Node structure for the linked list, holding a line and a pointer to the next node
struct line_node {
    struct line value;         // The line stored in this node
    struct line_node* next;    // Pointer to the next node in the list
};

// Structure for the linked list of lines
struct linked_list_of_lines {
    struct line_node* head;    // Pointer to the first node in the list
};

// Creates a new linked list and returns a pointer to it
struct linked_list_of_lines* linked_list_create(void);

/*
    Add a new line to the start of the linked list.
    Parameters:
        - struct linked_list_of_lines* list: The linked list to append the line to
        - struct line new_line: The line to be added to the list
*/
int linked_list_add_line(struct linked_list_of_lines* list, struct line new_line);

/*
    Destroys the linked list and frees all allocated resources.
    Parameters:
        - struct linked_list_of_lines* list: The linked list to be destroyed
*/
void linked_list_destroy(struct linked_list_of_lines* list);

#endif  // LINKED_LIST_H