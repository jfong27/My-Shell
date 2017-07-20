#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define START_SIZE 20

void run_shell();
char **read_cline();
int new_process(char **cmd);
int execute(char **args);
char *read_line(FILE *input);
void *checked_malloc(size_t spaces);
int num_builtins();
int my_cd(char **args);
int my_exit(char **args);

char *builtin_str[] = {
   "cd",
   "exit"
};
int (*builtin_func[]) (char **) = {
   &my_cd,
   &my_exit
};

