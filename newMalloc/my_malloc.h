/*
** my_malloc.h for  in /home/gandoulf/epitech/PSU_project/test/malloc_test
**
** Made by gandoulf
** Login   <gandoulf@epitech.net>
**
** Started on  Tue Jan 26 15:12:40 2016 gandoulf
** Last update Sat Feb  6 14:49:45 2016 Enzo Resse
*/

#ifndef MYMALLOC_H_
#define MYMALLOC_H_

#include <unistd.h>
#include <stdio.h>
#include <string.h>

//#include "my_printf.h"

#define _TRUE 1
#define _FALSE 0

typedef struct	__attribute__((packed))	s_metadata
{
  size_t				_allocSize;
  int					_used;
}					t_metadata;

void    *malloc(size_t size);
void	free(void *);
void	*realloc(void *, size_t);
void	show_alloc_mem();

void    *findMemory(void *start, void *end, size_t size);
void	*addMemory(void **end, void *ptr, size_t size);
void	useMemory(void *ptr, size_t size);
size_t	giveMemory(void *, size_t);

void	*increasMemory(void **ptr, size_t size, void *start, void **end);
void	reducedMemory(void *ptr, size_t size);

#endif
