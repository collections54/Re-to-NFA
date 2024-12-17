/* Contains functions for taking inputted RE and converting it to an 
 * nfa, as well as sorting and printing nfa structure. Also handles most
 * errors relating to RE input */


#include "nfa.h"

/* Converts a RE to nfa by calling nfa.c functions
 * and popping nfas from a stack. 
 * Returns 0 on success, -1 on failure.
 * Sends error messages to stderr */
int re_to_nfa(char * string)
{
 remove_newline(string); //remove trailing newline from string
 int length = strlen(string); //determine length of strings
 int statenum = 1; //initalize statenum to 1
 struct node *tos = init(); //initalize a stack
 for(int i = 0; i < length; i++) //loop for each character in string
 { 
  char c = string[i]; //get the current character
  switch(c)
  {
   case('a'): //all symbols in the alphabet
   case('b'):
   case('c'):
   case('d'):
   case('e'):
   case('E'):
    statenum = symbol(c, statenum, tos); //create a symbol nfa
    break;
   case('|'): //If c is an or
   {
    struct nfa * nfa1 = (struct nfa *) pop(tos); //pop 1st nfa
    if(nfa1 != NULL) //if stack was not empty 
    {
     struct nfa * nfa2 = (struct nfa *) pop(tos); //pop 2nd nfa
     if(nfa2 != NULL)
     {
      statenum = or_nfa(statenum, nfa1, nfa2, tos);
      break;
     }
     free_nfa(nfa1); //if there was only one nfa, free the nfa
    }
    fprintf(stderr, "\nERROR: Not enough arguments for | operation in RE <%s>\n", string); //output an error message
    free_stack(tos); //free the stack
    return(-1); //return -1 indicating an error
   }
   case('*'): //handle kleene star
   {
    struct nfa * nfa1 = (struct nfa *) pop(tos); //pop nfa1
    if(nfa1 != NULL)
    {
     statenum = kleenestar(statenum, nfa1, tos); //create a kleene star nfa
     break;
    }
    fprintf(stderr, "\nERROR: Not enough arguments for * operation in RE <%s>\n", string); //If the stack was empty
    free_stack(tos); //free the stack
    return(-1);
   }
   case('&'): //handle concatinations
   {
    struct nfa * nfa1 =  (struct nfa *) pop(tos); //pop 1st nfa
    if(nfa1 != NULL)
    {
     struct nfa * nfa2 = (struct nfa *) pop(tos); //pop 2nd nfa
     if(nfa2 != NULL)
     {
      statenum = concat(statenum, nfa1, nfa2, tos); //create a concat nfa
      break;
     }
     free_nfa(nfa1); //free 1st nfa if stack only contained one nfa
    }
    fprintf(stderr, "\nERROR: Not enough arguments for & operation in RE <%s>\n", string);
    free_stack(tos); //free the stack
    return(-1);
   }
   default: //handle if a character is not recognized
    fprintf(stderr, "\nERROR: Argument <%c> not recognized in RE <%s>\n", c, string);
    free_stack(tos); //free the stack
    return -1;
  }
 }
 struct nfa *nfa =  (struct nfa *) pop(tos); //pop what should be the last nfa
 if(isEmpty(tos)) //if the stack is empty then we are done
 {
  sort(nfa, string); //sort the nfa
  free(tos); //free the stack
  return(0); //return 0 to indicate success
 }
 //else stack was not empty and the RE was not formatted correctly
 fprintf(stderr, "\nERROR: Malformed input in RE <%s>\n", string); 
 free_nfa(nfa); //free the nfa
 free_stack(tos); //free everything on the stack
 return(-1);
}

// Frees all memory on the stack
void free_stack(struct node *tos)
{
 while(!isEmpty(tos)) //while the stack is not empty
 {
  struct nfa *nfaf = (struct nfa *) pop(tos); //pop the nfa
  free_nfa(nfaf); //free the nfa
 }
 free(tos); //free the tos
}

//Remove newline from input string
void remove_newline(char *string)
{
 char * ptr = string;
 char * newstring = string;
 while(*ptr != '\0') //while element at ptr != null terminator
 {
  if(*ptr != '\n' && *ptr != '\r') //copy over string
  {
   *newstring++ = *ptr;
  }
 ptr ++; //move allong the string
 }
 *newstring = '\0'; //set newline as null terminator
}

/* compare function for qsort.
 * Sorts by state number, with lowest start state first
 * If states are equal, sort by accept state */
int compare(const void *a, const void *b)
{
 struct edge * edge1 = *(struct edge **) a; //type cast first edge
 struct edge * edge2 = *(struct edge **) b; //type cast 2nd edge
 if(edge1->from < edge2->from) // edge1 should be before edge2
 {
  return -1;
 }
 else if(edge1->from > edge2->from) //edge1 should be after edge2
 {
  return 1;
 }
 if(edge1->next < edge2->next) // edge1 before edge2
 {
  return -1;
 }
 else if(edge1->next > edge2->next) //edge1 after edge 2
 {
  return(1);
 }
 return (0); //two edges are the same
}

/* Sorts edges on nfa, then prints out the output.
 * Sorts using qsort */
void sort(struct nfa * nfa, char *string)
{
 struct nfa * nfac = nfa; //create duplicate pointer
 qsort(nfac->edge, nfac->edgenum, sizeof(struct edge *), compare);
 printf("\n");
 printf("RE: %s\n", string);
 printf("Start: q%d\n", nfac->start);
 printf("Accept: q%d\n", nfac->accept);
 for(int i = 0; i < nfac->edgenum; i++) //print out in form (q1, a) → q2
 {
  printf("(q%d, %c) → q%d\n", nfac->edge[i]->from, nfac->edge[i]->symbol, nfac->edge[i]->next);
 }
 free_nfa(nfac); //free the nfa
}  	   


