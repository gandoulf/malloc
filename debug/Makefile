CC = gcc # C compiler
CFLAGS = -fPIC -Wall -Wextra -O2 -g -DDEBUG # C flags
LDFLAGS = -shared  # linking flags
RM = rm -f  # rm command
TARGET_LIB = libmalloc.so # target lib

SRCS = findplace.c my_malloc.c resignedMemory.c # source files
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS}