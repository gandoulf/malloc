##
## Makefile for  in /home/agor_m/github/malloc/newMalloc
## 
## Made by Maxime Agor
## Login   <agor_m@epitech.net>
## 
## Started on  Sat Feb  6 13:47:54 2016 Maxime Agor
## Last update Sun Feb 14 16:29:54 2016 gandoulf
##

CC = gcc # C compiler

CFLAGS = -fPIC -Wall -Wextra -O3 # C flags

#CFLAGS += -g -DDEBUG

CFLAGS += -DTHREAD

LDFLAGS = -shared -lpthread # linking flags

RM = rm -f  # rm command

TARGET_LIB = libmalloc.so # target lib

SRCS =	findMemory.c \
	my_malloc.c \
	reassignMemory.c

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
