gcc_opt = -pthread -Wall -Wextra
cla_opt = -pthread -Weverything
ifeq ($(CC),gcc)
	options = $(gcc_opt)
endif
ifeq ($(CC),clang)
	options = $(cla_opt)
endif

cputr: main.c cut_threads.c cut_threads.h main.h
	$(CC) main.c cut_threads.c $(options) -std=c99 -o cputr
clean:
	rm cputr
