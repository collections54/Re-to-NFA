/* Main function for re_to_nfa.
 * Handles command line input and file I/O.
 * Can be passed multiple files on the command line
 * Assumes one RE per line, continues if RE is invalid
 * or file cannot be opened */
#include "nfa.h"
#define BUFFSIZE 1024 //arbitrary buffer size

int main(int argc, char * argv[])
{
 if(argc < 2) //needs at least 1 file 
 {
  fprintf(stderr, "ERROR: No arguments given\n");
  exit(0);
 }
 for(int i = 1; i < argc; i++) //for each argument on command line
 {
  FILE *fp = fopen(argv[i], "r"); //try to open file for reading
  if(fp == NULL)
  {
   fprintf(stderr, "\nERROR opening %s: <%s>\n", argv[i], strerror(errno));
   continue; //move to next file
  } 
  char buffer[BUFFSIZE]; 
  while(fgets(buffer, BUFFSIZE, fp) != NULL) //read one line from file
  {
   if((strcmp(buffer, "\n") == 0) || (strcmp(buffer,"\r") == 0))
   {
    continue; //ignore newlines
   }
   int value = re_to_nfa(buffer); //convert RE to nfa, don't need to check
				  //return value of re_to_nfa
  }
  fclose(fp); //close the file
 }  
 exit(0); //exit with 0 to indicate success
}
