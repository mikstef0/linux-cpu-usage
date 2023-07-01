gcc_opt = -pthread -Wall -Wextra
cla_opt = -pthread -Weverything -Wno-missing-variable-declarations -Wno-disabled-macro-expansion -Wno-sign-conversion -Wno-padded
ifeq ($(CC),gcc)
	options = $(gcc_opt)
endif
ifeq ($(CC),clang)
	options = $(cla_opt)
endif

cputr: main.c cut_threads.c cut_threads.h main.h global.h
	$(CC) main.c cut_threads.c $(options) -std=c99 -o cputr
test: main_test.c cut_threads.c cut_threads.h main.h global.h
	$(CC) main_test.c $(options) -std=c99 -o test
clean:
	rm cputr
