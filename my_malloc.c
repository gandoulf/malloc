/*
** my_malloc.c for  in /home/resse_e/malloc
**
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
**
** Started on  Sat Feb  6 12:20:25 2016 Enzo Resse
** Last update Sun Feb 14 16:28:07 2016 gandoulf
*/

#include "my_malloc.h"

static void     *start = NULL;
static void     *end = NULL;
pthread_mutex_t	lock[4];

void    *malloc(size_t size)
{
#ifdef THREAD
  pthread_mutex_lock(&lock[0]);
#endif
#ifdef DEBUG
  printf("USE MALLOC !!!!! malloc %zu\n", size);
#endif
  void  *ptr;

  if (!size)
    {
#ifdef THREAD
      pthread_mutex_unlock(&lock[0]);
#endif
      return NULL;
    }
  if (start == NULL)
    {
      start = sbrk(0);
      sbrk(getpagesize());
      ((t_metadata *)start)->_allocSize = getpagesize();
      ((t_metadata *)start)->_used = 0;
      end = sbrk(0);
    }
  size += (size % sizeof(size_t)) ? sizeof(size_t) - (size % sizeof(size_t)) : 0;
  ptr = findMemory(start, end, size);
  if ((ptr = addMemory(&end, ptr, size)) == 0)
    {
#ifdef THREAD
      pthread_mutex_unlock(&lock[0]);
#endif
#ifdef DEBUG
      printf("add memory fail\n");
#endif
      return (0);
    }
  useMemory(ptr, size);
  ptr += sizeof(t_metadata);
#ifdef DEBUGMEM
  show_alloc_mem();
#endif
#ifdef DEBUG
  printf("END OF MALLOC\n");
#endif
#ifdef THREAD
  pthread_mutex_unlock(&lock[0]);
#endif
  return(ptr);
}

void	free(void *ptr)
{
#ifdef THREAD
  pthread_mutex_lock(&lock[1]);
#endif
#ifdef DEBUG
  printf("USE FREE !!!!!, free this : %p\n",ptr);
#endif
  if (ptr < start + sizeof(t_metadata) || ptr >= end)
    {
#ifdef THREAD
      pthread_mutex_unlock(&lock[1]);
#endif
      return;
    }
#ifdef THREAD
  pthread_mutex_lock(&lock[0]);
#endif
  ptr -= sizeof(t_metadata);
#ifdef THREAD
      pthread_mutex_unlock(&lock[0]);
#endif
  ((t_metadata *)ptr)->_used = 0;

#ifdef DEBUGMEM
  show_alloc_mem();
#endif
#ifdef DEBUG
  printf("END OF FREE\n");
#endif
#ifdef THREAD
      pthread_mutex_unlock(&lock[1]);
#endif
}

void	*realloc(void *ptr, size_t size)
{
#ifdef THREAD
      pthread_mutex_lock(&lock[2]);
#endif
#ifdef DEBUG
  printf("USE REALLOC !!!!! realloc this : %p, of %zu\n", ptr, size);
#endif
  if (ptr < start + sizeof(t_metadata) || ptr > end)
    {
#ifdef THREAD
      pthread_mutex_unlock(&lock[2]);
#endif
      return (malloc(size));
    }
  if (size == 0)
  {
    free(ptr);
#ifdef THREAD
    pthread_mutex_unlock(&lock[2]);
#endif
    return NULL;
  }
  ptr -= sizeof(t_metadata);
  size += (size % sizeof(size_t)) ? sizeof(size_t) - (size % sizeof(size_t)) : 0;
  if (((t_metadata *)ptr)->_allocSize < size + sizeof(t_metadata))
    {
      free(ptr + sizeof(t_metadata));
#ifdef THREAD
      pthread_mutex_lock(&lock[0]);
#endif
      if ((ptr = increaseMemory(ptr, size, start, &end)) == 0)
	{
#ifdef DEBUG
	  printf("add memory fail\n");
#endif
#ifdef THREAD
	  pthread_mutex_unlock(&lock[0]);
	  pthread_mutex_unlock(&lock[2]);
#endif
	  return (0);
	}
#ifdef THREAD
      pthread_mutex_unlock(&lock[0]);
#endif
    }
  else if (((t_metadata *)ptr)->_allocSize >= size + 2 * sizeof(t_metadata))
    reduceMemory(ptr, size);
  ptr += sizeof(t_metadata);
#ifdef DEBUGMEM
  show_alloc_mem();
#endif
#ifdef DEBUG
  printf("END OF REALLOC\n");
#endif
#ifdef THREAD
  pthread_mutex_unlock(&lock[2]);
#endif
  return (ptr);
}

void		*calloc(size_t nmemb, size_t size)
{
#ifdef THREAD
  pthread_mutex_lock(&lock[3]);
#endif
  void		*ptr;

#ifdef DEBUG
  printf("USE CALLOC !!!!! nmemb = %zu, size  %zu\n", nmemb, size);
#endif
  if (!(ptr = malloc(nmemb * size)))
    {
#ifdef THREAD
      pthread_mutex_unlock(&lock[0]);
#endif
      return NULL;
    }
#ifdef DEBUG
  printf("AFTER MALLOC CALL !!!!! ptr = %p\n", ptr);
#endif
  memset(ptr, 0, nmemb * size);
#ifdef DEBUGMEM
  show_alloc_mem();
#endif
#ifdef DEBUG
  printf("END OF CALLOC\n");
#endif
#ifdef THREAD
  pthread_mutex_unlock(&lock[3]);
#endif
  return (ptr);
}

void	show_alloc_mem()
{
  void		*ptr = start;

  printf("break : %p\n", end);
  while (ptr != end)
    {
      printf("%s", ((t_metadata *)ptr)->_used ? "\033[31m" : "\033[32m");
      printf("%p - %p : %zu", ptr + sizeof(t_metadata),
	     ptr + ((t_metadata *)ptr)->_allocSize,
	     ((t_metadata *)ptr)->_allocSize - sizeof(t_metadata));
      ptr += ((t_metadata *)ptr)->_allocSize;
      printf("\033[37m\n");
    }
}
