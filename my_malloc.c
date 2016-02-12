/*
** my_malloc.c for  in /home/resse_e/malloc
** 
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
** 
** Started on  Sat Feb  6 12:20:25 2016 Enzo Resse
** Last update Fri Feb 12 15:23:22 2016 Enzo Resse
*/

#include "my_malloc.h"

static void     *start = NULL;
static void     *end = NULL;

void    *malloc(size_t size)
{
#ifdef DEBUG
  printf("USE MALLOC !!!!! malloc %zu\n", size);
#endif
  void  *ptr;

  if (!size)
    return NULL;
  if (start == NULL)
    {
      start = sbrk(0);
      sbrk(getpagesize());
      ((t_metadata *)start)->_allocSize = getpagesize();
      ((t_metadata *)start)->_used = 0;
      end = sbrk(0);
    }
  size += (size % sizeof(int)) ? sizeof(int) - (size % sizeof(int)) : 0;
  ptr = findMemory(start, end, size);
  if ((ptr = addMemory(&end, ptr, size)) == 0)
    {
#ifdef DEBUG
      printf("add memory fail\n");
#endif
      return (0);
    }
  useMemory(ptr, size);
  ptr += sizeof(t_metadata);
#ifdef DEBUG
  show_alloc_mem();
#endif
  return(ptr);
}

void	free(void *ptr)
{
#ifdef DEBUG
  printf("USE FREE !!!!!, free this : %p\n",ptr);
#endif
  if (ptr < start + sizeof(t_metadata) || ptr >= end)
    return;
  ptr -= sizeof(t_metadata);
  ((t_metadata *)ptr)->_used = 0;

#ifdef DEBUG
  show_alloc_mem();
#endif
}

void	*realloc(void *ptr, size_t size)
{
#ifdef DEBUG
  printf("USE REALLOC !!!!! realloc this : %p, of %zu\n", ptr, size);
#endif
  if (ptr < start + sizeof(t_metadata) || ptr > end)
    return (malloc(size));
  if (size == 0)
  {
    free(ptr);
    return NULL;
  }
  ptr -= sizeof(t_metadata);
  size += (size % sizeof(int)) ? sizeof(int) - (size % sizeof(int)) : 0;
  if (((t_metadata *)ptr)->_allocSize < size + sizeof(t_metadata))
    {
      if ((ptr = increaseMemory(ptr, size, start, &end)) == 0)
	return (0);
    }
  else if (((t_metadata *)ptr)->_allocSize >= size + 2 * sizeof(t_metadata))
    reduceMemory(ptr, size);
  ptr += sizeof(t_metadata);
#ifdef DEBUG
  show_alloc_mem();
#endif
  return (ptr);
}

void		*calloc(size_t nmemb, size_t size)
{
  void		*ptr;

#ifdef DEBUG
  printf("USE CALLOC !!!!! nmemb = %zu, size  %zu\n", nmemb, size);
#endif
  if (!(ptr = malloc(nmemb * size)))
    return NULL;
#ifdef DEBUG
  printf("AFTER MALLOC CALL !!!!! ptr = %p\n", ptr);
#endif
  memset(ptr, 0, nmemb * size);
#ifdef DEBUG
  show_alloc_mem();
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
