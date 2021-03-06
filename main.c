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
   int num_args = 10;
   char *command;
   char **cmd_args = checked_malloc(sizeof(char*) * num_args);

   printf(">> ");
   command = read_line(stdin);

   split_command(command, cmd_args);
   return cmd_args;

}

void split_command(char *command, char **args){
   int arg_pos = 0;
   int curr_arg_pos = 0;
   int quote = 0;
   int arg_len = 10;
   char *cmd = command;
   char *curr_arg = checked_malloc(sizeof(char*)* arg_len);
   
   while(*cmd != '\0'){
      if(*cmd == '"'){
         if(quote == 0){
            /*start a quote*/
            quote = 1;
         }
         else{
            /*quote ending, end arg*/
            curr_arg[curr_arg_pos] = '\0';
            args[arg_pos++] = curr_arg;
            curr_arg = checked_malloc(sizeof(char*) * 10);
            curr_arg_pos = 0;
            arg_len = 10;
         }

      }
      else if(*cmd == ' ' && quote == 0){
         /*end arg*/
         curr_arg[curr_arg_pos] = '\0';
         args[arg_pos++] = curr_arg;
         curr_arg = checked_malloc(sizeof(char*) * 10);
         curr_arg_pos = 0;
         arg_len = 10;
      }
      else{
         curr_arg[curr_arg_pos++] = *cmd;
         if(curr_arg_pos >= arg_len - 1){
            arg_len += 5;
            curr_arg = realloc(curr_arg, arg_len);
         }
         
      }
      cmd++;
   }
   curr_arg[curr_arg_pos] = '\0';
   args[arg_pos] = curr_arg;

   args[arg_pos+1] = 0;

   free(command);


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
      if(strcmp(cmd[0],"tree")==0){
         execl("./tree", "cmd");
         perror("Tree");
      }
      else{
         execvp(cmd[0],cmd);
         perror("Exec");
         exit(-1);
      }
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
      status = 0;
   }
   else{
      status = chdir(args[1]);
      if(status != 0)
         fprintf(stderr, "cd: %s: No such file or directory\n", args[1]);
   }

   return status;
}

int my_exit(){
   printf("Closing shell...\n");

   return -1;
}

int my_ls(char **args){
   
   printf("ls\n");
   return 0;
}

int my_pwd(char **args){

   printf("%s\n",getcwd(0,0));

   return 0;
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


