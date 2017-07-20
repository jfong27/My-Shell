CFLAGS = -Wall -pedantic 

all: my_shell

my_shell: main.o
	gcc $(CFLAGS) main.o -o my_shell

main.o: main.c main.h
	gcc $(CFLAGS) main.c -c
