/**************************************************
 *
 *    I am following Stephen Brennan's tutorial
 *    on how to write a shell.
 *
 *************************************************/

#include "main.h"


/**************************************************
 *
 * Function: run_shell
 * -------------------
 *    Main function which loops, reading commands
 *    from the command prompt.
 *
 *************************************************/
void run_shell(){

   char **cmd = NULL;
   int status = 1;
   while(status != -1){
      cmd = read_cline();

      status = -1; 
   }

}

/**************************************************
 *
 * Function: read_cline
 * -------------------
 *   Reads the command line, parses it into array
 *   of strings. 
 *
 *************************************************/

char **read_cline(){
   char *command;
   char *split;
   char **cmd_args = checked_malloc(sizeof(char*) * 10);
   int i = 0;

   printf(">> ");
   command = read_line(stdin);

   printf("command: %s\n", command);

   split = strtok(command, " ");
   while(split != NULL){
      cmd_args[i++] = split;
      split = strtok(NULL, " ");
   }
   return cmd_args;

}


char *read_line(FILE *input){
   int buffer_size = START_SIZE;
   char *buffer = (char*)checked_malloc(buffer_size);
   char c = getc(input);
   int pos = 0;

   if (c == EOF){
      free(buffer);
      return NULL;
   }
   while (c != '\n' && c != EOF && c != 0){
      *(buffer+pos) = c;
      pos++;
      if(pos == buffer_size){
         buffer_size *= 2;
         buffer = (char*)realloc(buffer, buffer_size * 2);
      }
      c = getc(input);
   }
   *(buffer+pos) = '\0';
   
   return buffer;
}

void *checked_malloc(size_t spaces){
   void *space = malloc(spaces);
   if(space == NULL){
      perror("Malloc");
      exit(-1);
   }
   return space;
}
int main(int argc, char *argv[]){

   run_shell();

   return 0;
}


