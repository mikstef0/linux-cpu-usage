gcc_opt = -pthread -Wall -Wextra
cla_opt = -pthread -Weverything
ifeq ($(CC),gcc)
	options = $(gcc_opt)
endif
ifeq ($(CC),clang)
	options = $(cla_opt)
endif

cputr: main.c threads.c header.h
	$(CC) main.c threads.c $(options) -std=c99 -o cputr
clean:
	rm cputr
