##
## Makefile for  in /home/agor_m/github/malloc/newMalloc
## 
## Made by Maxime Agor
## Login   <agor_m@epitech.net>
## 
## Started on  Sat Feb  6 13:47:54 2016 Maxime Agor
## Last update Sat Feb  6 15:54:45 2016 Maxime Agor
##

CC = gcc # C compiler

CFLAGS = -fPIC -Wall -Wextra -O2 # C flags

CFLAGS += -g -DDEBUG

LDFLAGS = -shared # linking flags

RM = rm -f  # rm command

TARGET_LIB = libmalloc.so # target lib

SRCS =	findMemory.c \
	my_malloc.c \
	resignedMemory.c

OBJS = $(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

.PHONY: clean
clean:
	-${RM} ${OBJS}

.PHONY: fclean
fclean: clean
	-${RM} ${TARGET_LIB}

.PHONY: re
re:	fclean all
