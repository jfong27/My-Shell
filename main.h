#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define START_SIZE 20

void run_shell();
char **read_cline();
char *read_line(FILE *input);
void *checked_malloc(size_t spaces);


