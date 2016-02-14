/*
** initMalloc.c for  in /home/agor_m/malloc
** 
** Made by Maxime Agor
** Login   <agor_m@epitech.net>
** 
** Started on  Sun Feb 14 17:07:02 2016 Maxime Agor
** Last update Sun Feb 14 18:39:21 2016 Maxime Agor
*/

#include "my_malloc.h"

inline void	init_malloc(void **start, void **end, void **ptr)
{
  *start = sbrk(0);
  sbrk(getpagesize());
  ((t_metadata *)*start)->_allocSize = sizeof(t_metadata);
  ((t_metadata *)*start)->_prevFree = 0;
  ((t_metadata *)*start)->_nextFree = *start + sizeof(t_metadata);
  ((t_metadata *)*start)->_nextElem = *start + sizeof(t_metadata);
  ((t_metadata *)*start)->_properties = 0;
  *end = sbrk(0);
  *ptr = *start + sizeof(t_metadata);
  ((t_metadata *)*ptr)->_allocSize = getpagesize() - sizeof(t_metadata);
  ((t_metadata *)*ptr)->_prevFree = *start;
  ((t_metadata *)*ptr)->_nextFree = *end;
  ((t_metadata *)*ptr)->_nextElem = *end;
  ((t_metadata *)(*start + sizeof(t_metadata)))->_properties = 0;
}

inline void	*unlock_mutexes_0_and_2(pthread_mutex_t *lock)
{
  pthread_mutex_unlock(&lock[0]);
  pthread_mutex_unlock(&lock[2]);
  return (0);
}

inline void	*unlock_mutex_2_free_ptr_and_return_null(pthread_mutex_t *lock,
							 void *ptr)
{
  pthread_mutex_unlock(&lock[2]);
  free(ptr);
  return (0);
}

inline void	create_link(void **tmp, void **ptr,
			    size_t size, size_t nextData)
{
  ((t_metadata *)*ptr)->_prevFree->_nextFree = *tmp;
  ((t_metadata *)*tmp)->_allocSize = nextData - (size + sizeof(t_metadata));
  ((t_metadata *)*tmp)->_nextFree = ((t_metadata *)*ptr)->_nextFree;
  ((t_metadata *)*tmp)->_prevFree = ((t_metadata *)*ptr)->_prevFree;
  ((t_metadata *)*tmp)->_nextElem = ((t_metadata *)*ptr)->_nextElem;
  ((t_metadata *)*tmp)->_properties = 0;
  ((t_metadata *)*ptr)->_nextElem = *tmp;
}

inline int	add_pagesize(size_t size, void *ptr)
{
  size_t	space;

  space = SPACE(getpagesize(), size + (2 * sizeof(t_metadata)), ptr);
  if (sbrk(space) == (void *) -1)
    return (0);
  ((t_metadata *)ptr)->_allocSize += space;
  ((t_metadata *)ptr)->_nextFree = sbrk(0);
  ((t_metadata *)ptr)->_nextElem = sbrk(0);
  return (1);
}
