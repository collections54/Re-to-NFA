/*
 * Generic stack program using linked list structure
 * Uses a struct node sent which is the tos pointer
 */

#include "stack.h"

/* Intiialize a stack pointer sent and return it */
struct node * init()
{
 struct node *sent = malloc(sizeof(struct node));
 sent->data = NULL;
 sent->next = NULL;
 return(sent);
}

/*
 * Pop from the stack. Returns void * data on success
 * NULL on failure
 */

void *pop(struct node * sent)
{
 if(isEmpty(sent))
 {
  return(NULL);
 }
 void * data;
 struct node *temp = sent->next; // move stack pointer down
 sent->next = temp->next;
 data = temp->data;
 free(temp); //free popped node
 return(data);
}

/* Checks if stack is empty */
bool isEmpty(struct node * sent)
{
 return(sent->next == NULL);
}

/* Push data onto a new struct node and push to tos */
void push(struct node *sent, void * n)
{
 struct node *newnode = malloc(sizeof(struct node));
 newnode->data = n;
 newnode->next = sent->next;
 sent->next = newnode;
}

