/** @file list.h
 *  @brief Function prototypes for the linked list.
 * 
 * modified to include more useful functions for linked lists 
 * 
 */
#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#define MAX_WORD_LEN 50

/**
 * @brief An struct that represents a node in the linked list.
 * 
 */
typedef struct node_t
{
    char *word;
    struct node_t *next;
} node_t;

/**
 * Function protypes associated with a linked list.
 * 
 */
node_t *new_node(char *val);
node_t *add_front(node_t *, node_t *);
node_t *add_end(node_t *, node_t *);
node_t *add_inorder(node_t *, node_t *);
node_t *peek_front(node_t *);
node_t *remove_front(node_t *);
void apply(node_t *, void (*fn)(node_t *, void *), void *arg);

void inccounter(node_t *p, void *arg);
void print_node(node_t *p, void *arg);
void analysis(node_t *l);
void free_list(node_t *head);
node_t *reverse_list(node_t *head);

#endif
