CFLAGS := -pthread -Wall -Wextra

ifeq ($(CC),gcc)
    CFLAGS += -Wno-missing-variable-declarations
endif

ifeq ($(CC),clang)
    CFLAGS += -Weverything -Wno-disabled-macro-expansion -Wno-sign-conversion -Wno-padded -Wno-redundant-parens -Wno-strict-prototypes -Wno-missing-noreturn -Wno-missing-variable-declarations
endif

SRCS := main.c cut_threads.c cut_queue.c
OBJS := $(SRCS:.c=.o)
HEADERS := cut_threads.h global.h cut_queue.h

.PHONY: all clean

all: cputr test

cputr: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -std=c11 -o cputr

test: main_test.c $(OBJS)
	$(CC) main_test.c $(CFLAGS) -std=c11 -o test

$(OBJS): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -std=c11 -c $(SRCS)

clean:
	rm -f cputr test $(OBJS)
