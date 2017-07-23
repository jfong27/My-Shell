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
   /*Still have to free cmd args*/
   char **cmd = NULL;
   int status = 0;
   while(status != -1){
      cmd = read_cline();
      status = execute(cmd);
   }

   free(cmd);
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

   split = strtok(command, " ");
   while(split != NULL){
      cmd_args[i++] = split;
      split = strtok(NULL, " ");
   }
   return cmd_args;

}
/**************************************************
 *
 * Function: new_process
 * -------------------
 *   Forks a new process and execs the 
 *   desired command. 
 *
 *************************************************/

int new_process(char **cmd){
   int status;
   pid_t pid; 

   pid = fork();
   if(pid == 0){
      if(strcmp(cmd[0],"tree")==0)
         execl("./tree", "cmd");
      execvp(cmd[0],cmd);
      perror("Exec");
   }
   else if(pid > 0){
      waitpid(pid,&status,0);
   }
   else{
      perror("Fork");
   }

   return 0;
}


int execute(char **args){
   int builtin = 1;
   int i;
   int status = 0;

   if(args[0] != NULL){
      for(i = 0; i < num_builtins(); i++){
         if(strcmp(args[0], builtin_str[i])==0){
            status = (*builtin_func[i])(args);
            builtin = 0;
         }
      }
      if(builtin == 1)
         status = new_process(args);
   }
   else{
      status = -1;
   }

   return status;


}
/**************************************************
 *
 * Function: read_line
 * -------------------
 *   Reads a line from file pointer input.
 *   Allocates memory for the line, dynamically.  
 *
 *************************************************/

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

int num_builtins(){
   return sizeof(builtin_str) / sizeof(char*);
}

int my_cd(char **args){
   int status;

   if(args[1] == NULL){
      fprintf(stderr, "Usage: cd [directory]\n");
      status = -1;
   }
   else{
      status = chdir(args[1]);
      if(status != 0)
         fprintf(stderr, "cd: %s: No such file or directory\n", args[1]);
   }

   return status;
}

int my_exit(char **args){
   printf("Closing shell...\n");

   return -1;
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


