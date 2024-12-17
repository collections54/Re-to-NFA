/* Header file for stack functions. Includes stack function definition and
 * structure node definition
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
struct node{ //Structure definition for stack node structure.
 void * data; //generic stack data
 struct node * next;
};

struct node * init(); //prototype for initalizing stack
void push(struct node *tos, void * data); //prototype for pushing stack
void *pop(struct node *sent); //prototype for poping from stack
bool isEmpty(struct node *tos); //prototype for checking if stack is empty



