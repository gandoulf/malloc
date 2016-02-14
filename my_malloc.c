/*
** my_malloc.c for  in /home/resse_e/malloc
**
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
**
** Started on  Sat Feb  6 12:20:25 2016 Enzo Resse
** Last update Sun Feb 14 17:41:19 2016 Maxime Agor
*/

#include "my_malloc.h"

static void     *start = NULL;
static void     *end = NULL;
pthread_mutex_t	lock[4];

void    *malloc(size_t size)
{
  pthread_mutex_lock(&lock[0]);
  void  *ptr;

  if (!size)
    {
      pthread_mutex_unlock(&lock[0]);
      return NULL;
    }
  if (start == NULL)
    init_malloc(&start, &end, &ptr);
  size += (size % sizeof(size_t)) ?
    sizeof(size_t) - (size % sizeof(size_t)) : 0;
  ptr = findMemory(start, end, size);
  if ((ptr = addMemory(&end, ptr, size)) == 0)
    {
      pthread_mutex_unlock(&lock[0]);
      return (0);
    }
  useMemory(ptr, size);
  ptr += sizeof(t_metadata);
  pthread_mutex_unlock(&lock[0]);
  return (ptr);
}

void	free(void *ptr)
{
  pthread_mutex_lock(&lock[1]);
  void	*tmp;

  tmp = start;
  if (ptr < (void *)sizeof(t_metadata))
    return (void)(pthread_mutex_unlock(&lock[1]));
  ptr -= sizeof(t_metadata);
  if (ptr < start || ptr >= end ||
      GET_VALUE(((t_metadata *)ptr)->_properties, _USED) == 0)
    return ((void)(pthread_mutex_unlock(&lock[1])));
  pthread_mutex_lock(&lock[0]);
  SET_VALUE(((t_metadata *)ptr)->_properties, _USED, 0);
  while (tmp < ptr && ((t_metadata *)tmp)->_nextFree <  (t_metadata *)ptr)
    tmp = ((t_metadata *)tmp)->_nextFree;
  if (((t_metadata *)tmp)->_nextFree != end)
    ((t_metadata *)tmp)->_nextFree->_prevFree = ptr;
  ((t_metadata *)ptr)->_prevFree = tmp;
  ((t_metadata *)ptr)->_nextFree = ((t_metadata *)tmp)->_nextFree;
  ((t_metadata *)tmp)->_nextFree = ptr;
  pthread_mutex_unlock(&lock[0]);
  pthread_mutex_unlock(&lock[1]);
}

void	*realloc(void *ptr, size_t s)
{
  pthread_mutex_lock(&lock[2]);
  if (ptr < start + sizeof(t_metadata) || ptr > end)
    {
      pthread_mutex_unlock(&lock[2]);
      return (malloc(s));
    }
  if (s == 0)
    return (unlock_mutex_2_free_ptr_and_return_null(lock, ptr));
  ptr -= sizeof(t_metadata);
  s += (s % sizeof(size_t)) ? sizeof(size_t) - (s % sizeof(size_t)) : 0;
  if (((t_metadata *)ptr)->_allocSize < s + sizeof(t_metadata))
    {
      free(ptr + sizeof(t_metadata));
      pthread_mutex_lock(&lock[0]);
      if ((ptr = increaseMemory(ptr, s, start, &end)) == 0)
	return (unlock_mutexes_0_and_2(lock));
      pthread_mutex_unlock(&lock[0]);
    }
  else if (((t_metadata *)ptr)->_allocSize >= s + 2 * sizeof(t_metadata))
    reduceMemory(ptr, s);
  ptr += sizeof(t_metadata);
  pthread_mutex_unlock(&lock[2]);
  return (ptr);
}

void		*calloc(size_t nmemb, size_t size)
{
  pthread_mutex_lock(&lock[3]);
  void		*ptr;

  if (!(ptr = malloc(nmemb * size)))
    {
      pthread_mutex_unlock(&lock[3]);
      return NULL;
    }
  memset(ptr, 0, nmemb * size);
  pthread_mutex_unlock(&lock[3]);
  return (ptr);
}

void	show_alloc_mem()
{
  void		*ptr;

  ptr = start;
  printf("break : %p\n", end);
  while (ptr != end)
    {
      if (GET_VALUE(((t_metadata *)ptr)->_properties, _JUMPED))
	printf("\033[36m");
      else
	printf("%s",
	       GET_VALUE(((t_metadata *)ptr)->_properties, _USED) ?
	       "\033[31m" :
	       "\033[32m");
      printf("%p - %p : %zu", ptr + sizeof(t_metadata),
	     ptr + ((t_metadata *)ptr)->_allocSize,
	     ((t_metadata *)ptr)->_allocSize - sizeof(t_metadata));
      printf("\t: prev = %p, next = %p",
	     ((t_metadata *)ptr)->_prevFree,
	     ((t_metadata *)ptr)->_nextFree);
      printf("\t: properties = %d", ((t_metadata *)ptr)->_properties);
      printf("\033[37m\n");
      ptr = ((t_metadata *)ptr)->_nextElem;
    }
}
