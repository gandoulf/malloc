/*
** my_malloc.h for  in /home/gandoulf/epitech/PSU_project/test/malloc_test
**
** Made by gandoulf
** Login   <gandoulf@epitech.net>
**
** Started on  Tue Jan 26 15:12:40 2016 gandoulf
** Last update Sun Feb 14 18:09:55 2016 Maxime Agor
*/

#ifndef MYMALLOC_H_
#define MYMALLOC_H_

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define GET_VALUE(X, BIT) (((X) >> (BIT)) & 1)
#define SET_VALUE(X, B, V) (X) = (V) ? (X) | (1 << (B)) : (X) & ~(1 << (B))

#define _USED 0
#define _LOCKED 1
#define _JUMPED 2

typedef struct	__attribute__((packed))	s_metadata
{
  size_t				_allocSize;
  struct s_metadata			*_prevFree;
  struct s_metadata			*_nextFree;
  struct s_metadata			*_nextElem;
  int					_properties;
}					t_metadata;

void		*malloc(size_t size);
void		free(void *);
void		*realloc(void *, size_t);
void		show_alloc_mem();
void		*calloc(size_t, size_t);

void		*findMemory(void *start, void *end, size_t size);
void		*addMemory(void **end, void *ptr, size_t size);
void		jumpMemory(void *ptr, void *breakPoint, size_t space);
void		useMemory(void *ptr, size_t size);
inline void	mergeMemory(void **memPosition, size_t *memory,
			    void **ptr, void **last);

void		*increaseMemory(void *ptr, size_t size, void *start, void **end);
void		reduceMemory(void *ptr, size_t size);

inline void	init_malloc(void **start, void **end, void **ptr);
inline void	*unlock_mutexes_0_and_2(pthread_mutex_t *lock);
inline void     create_link(void **tmp, void **ptr,
			    size_t size, size_t nextData);
inline void	*unlock_mutex_2_free_ptr_and_return_null(pthread_mutex_t *lock,
							 void *ptr);

#endif
