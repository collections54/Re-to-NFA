/* Main headerfile for nfa operations. Contains structure definitions
 * and function prototypes for nfa.c and evaluate.c */

#include "stack.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>

struct edge{ //Structure for transitions
 char symbol; //symbol used to transition
 int from; //where transition starts
 int next; //where transition goes to 
};

struct nfa{ //Structure for nfa
 int start; //start state of nfa
 int accept; //accept state of nfa
 int edgenum; //number of transitons
 struct edge ** edge; //array of all transitons
};

//nfa.c functions
//create a new edge
void enew(struct edge *newedge, char symbol, int from, int next);
//initialize nfa
struct nfa *init_nfa(int start, int accept, int edgenum);
//free nfa as well as all edges
void free_nfa(struct nfa *nfa);
//function for handling a symbol nfa
int symbol(char symbol, int statenum, struct node *tos);
//function for handling an or of two nfas
int or_nfa(int statenum, struct nfa *nfa1, struct nfa *nfa2, struct node *tos);
//function for handling a kleenestar nfa
int kleenestar(int statenum, struct nfa *nfa1, struct node *tos);
//function for handling a concationation of two nfas
int concat(int statenum, struct nfa *nfa1, struct nfa *nfa2, struct node *tos);

//evaluate.c functions
//main function for creating the nfa
int re_to_nfa(char *string);
//compare function for qsort
int compare(const void *a, const void *b);
//sort edges and print output
void sort(struct nfa * nfa, char * string);
//remove trailing newline from input string
void remove_newline(char * string);
//free the stack
void free_stack(struct node * tos);

