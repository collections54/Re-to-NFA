/* nfa.c contains code for creating nfas for each case
 * each operator function pushes the nfa to the stack
 * and returns a running total number of states */


#include "nfa.h"

/* Create a new edge using given edge address, symbol, inital state
 * transition state.*/

void e_new(struct edge * newedge, char symbol, int from, int next)
{
 newedge->symbol = symbol; //set symbol char
 newedge->from = from; //set initial state
 newedge->next = next; //set next state
}

/* Initalizes an nfa and allocates memory for the nfa as well as
 * the edges using given start, accept and number of edges */
struct nfa * init_nfa(int start, int accept, int edgenum)
{
 struct nfa *nfa = malloc(sizeof(struct nfa)); //allocate space for nfa
 nfa->start = start;
 nfa->accept = accept;
 nfa->edgenum = edgenum;
 nfa->edge = malloc(edgenum *sizeof(struct edge *));
 for(int i = 0; i < edgenum; i++) //create an array on the heap
 {
  nfa->edge[i] = malloc(sizeof(struct edge)); 
 }
 return(nfa);
}

/* frees all memory associated with the nfa as well as
 * each edge and the edge pointer */
void free_nfa(struct nfa *nfa)
{
 if(nfa != NULL) //make sure nfa exists
 {
  for(int i = 0; i < nfa->edgenum; i++) 
  {
   free(nfa->edge[i]); //free each edge
  }
  free(nfa->edge); //free the edge pointer
  free(nfa); //finally free the nfa
 }
}

/* Creates an nfa recognizing a single symbol (q1, symbol) -> q2 
 * Can be though of as the base case since all other functions
 * require at least one nfa on the stack and creates 1 edge */
int symbol(char symbol, int statenum, struct node *tos )
{
 int start = statenum; //assign the start state
 int accept = statenum + 1; //assign the accept state
 statenum += 2; //increment the statenum count
 struct nfa *newnfa = init_nfa(start, accept, 1); //create the nfa
 e_new(newnfa->edge[0],symbol,start,accept); //create the edge from q1 to q2
 push(tos, newnfa); //push the nfa onto the stack
 return(statenum); //return the statenumber
}

/* Creates an nfa that recognizes the union of two nfas.
 * Requires two nfas on the stack and creates an additional 4 edges
 * for connecting the two nfas with epsilon transitions */
int or_nfa(int statenum,struct nfa * nfa1,struct nfa * nfa2, struct node *tos)
{
 int edgenum = nfa1->edgenum + nfa2->edgenum + 4; //total edges used by nfa
 int start = statenum;
 int accept = statenum + 1;
 statenum += 2; //create two new states, start and accept
 struct nfa * newnfa = init_nfa(start, accept, edgenum); //initalize the nfa
 newnfa->start = start; //assign the start
 newnfa->accept = accept; //assign the accept
 for(int i = 0; i < nfa1->edgenum; i++) //copy edges from nfa1
 {
  e_new(newnfa->edge[i], nfa1->edge[i]->symbol, nfa1->edge[i]->from, nfa1->edge[i]->next); 
 }
 int off = nfa1->edgenum; //need to make an offset to not overwrite nfa1 edges
 for(int i = 0; i < nfa2->edgenum ; i++) //copy over nfa2s edges
 {
  e_new(newnfa->edge[i+off], nfa2->edge[i]->symbol, nfa2->edge[i]->from, nfa2->edge[i]->next);
 }
 off += nfa2->edgenum; //offset to end of where nfa2s edges are located
 //Create a new epsilon transition from start state to start of nfa1
 e_new(newnfa->edge[off], 'E', newnfa->start, nfa1->start);
 //create a new epsilon transition from start state to start of nfa2
 e_new(newnfa->edge[off+1], 'E', newnfa->start, nfa2->start);
 //create a new epsilon transiton from accept state of nfa1 to accept of nfa
 e_new(newnfa->edge[off+2], 'E', nfa1->accept, newnfa->accept);
 //create a new epsilon transiton from accept state of nfa2 to accept of nfa
 e_new(newnfa->edge[off+3], 'E', nfa2->accept, newnfa->accept);
 push(tos, newnfa); //push nfa to stack
 free_nfa(nfa1); //free the first nfa
 free_nfa(nfa2); //free the second nfa
 return(statenum);
}
 
/* Creates an nfa that recognizes kleenestar of an nfa
 * Requires one nfa on the stack and creates two additional
 * edges for the loop */
int kleenestar(int statenum,struct nfa * nfa1, struct node *tos)
{
 int start = statenum; //only needs to create one additional state
 statenum ++;
 int edgenum = nfa1->edgenum +2; //needs two additonal edges
 struct nfa *newnfa = init_nfa(start, start, edgenum);
 for(int i = 0; i < nfa1->edgenum; i++) //copy over edges from nfa1
 {
  e_new(newnfa->edge[i], nfa1->edge[i]->symbol, nfa1->edge[i]->from, nfa1->edge[i]->next);
 }
 //Create an epsilon transition from start state to start state of nfa1
 e_new(newnfa->edge[edgenum-2], 'E', newnfa->start, nfa1->start);
 //Create an epsilon transition from accept state of nfa1 to start of nfa
 e_new(newnfa->edge[edgenum-1], 'E', nfa1->accept, newnfa->start);
 push(tos, newnfa); //push the nfa to stack
 free_nfa(nfa1); //free nfa1
 return(statenum);
}

/* Creates an nfa that recognizes the concationation of two nfas
 * requires two nfas be on stack and creates one additonal edge
 * to connect the two nfas */
int concat(int statenum, struct nfa * nfa1, struct nfa * nfa2, struct node *tos)
{
 int start = nfa2->start; //nfa2 is pushed onto stack first, popped second
 int accept = nfa1->accept; //nfa1 is pushed second, popped first
 int edgenum = nfa1->edgenum + nfa2->edgenum + 1; //need additonal edge
 struct nfa * newnfa = init_nfa(start, accept, edgenum);
 for(int i = 0; i < nfa1->edgenum; i++) //copy nfa1s edges
 {
  e_new(newnfa->edge[i], nfa1->edge[i]->symbol, nfa1->edge[i]->from, nfa1->edge[i]->next);
 }
 int off = nfa1->edgenum; //create offset to store nfa2s edges
 for(int i = 0; i < nfa2->edgenum; i++) //copy over nfa2s edges
 {
  e_new(newnfa->edge[i+off], nfa2->edge[i]->symbol, nfa2->edge[i]->from, nfa2->edge[i]->next);
 }
 //create epsilon transiton connecting nfa2s accept to nfa1s start
 e_new(newnfa->edge[edgenum-1], 'E', nfa2->accept, nfa1->start);
 push(tos, newnfa); //push the stack
 free_nfa(nfa1); //free nfa1
 free_nfa(nfa2); //free nfa2
 return(statenum);
}

