CC := gcc

cputr: main.c
	${CC} main.c threads.c -pthread -std=c99 -o cputr
