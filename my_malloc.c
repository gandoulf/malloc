/*
** my_malloc.c for  in /home/resse_e/malloc
**
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
**
** Started on  Sat Feb  6 12:20:25 2016 Enzo Resse
** Last update Tue Feb  9 14:25:57 2016 gandoulf
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
      ((t_metadata *)start)->_allocSize = sizeof(t_metadata);
      ((t_metadata *)start)->_prevFree = 0;
      ((t_metadata *)start)->_nextFree = start + sizeof(t_metadata);
      ((t_metadata *)start)->_nextElem = start + sizeof(t_metadata);
      SET_VALUE(((t_metadata *)start)->_properties, _USED, 0);
      printf("start->nextFree = %p\n", ((t_metadata *)start)->_nextFree);

      end = sbrk(0);
      ptr = start + sizeof(t_metadata);
      ((t_metadata *)ptr)->_allocSize = getpagesize() - sizeof(t_metadata);
      ((t_metadata *)ptr)->_prevFree = start;
      ((t_metadata *)ptr)->_nextFree = end;
      ((t_metadata *)ptr)->_nextElem = end;
      SET_VALUE(((t_metadata *)start + sizeof(t_metadata))->_properties, _USED, 0);
    }
  printf("start = %p, end = %p\n", start, end);
  printf("malloc need memory size = %zu\n", size);
  size += (size % sizeof(int)) ? sizeof(int) - (size % sizeof(int)) : 0;
  printf("try to find memory\n");
  ptr = findMemory(start, end, size);
  printf("memory found at %p\n", ptr);
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
  void	*tmp = start;
#ifdef DEBUG
  printf("USE FREE !!!!!, free this : %p\n",ptr);
#endif
  if (ptr < start + sizeof(t_metadata) || ptr >= end)
    return;
  ptr -= sizeof(t_metadata);
  ((t_metadata *)ptr)->_properties = 0;
  //SET_VALUE(((t_metadata *)ptr)->_properties, _USED, 1);
  while (tmp < ptr && ((t_metadata *)tmp)->_nextFree <  (t_metadata *)ptr)
    tmp = ((t_metadata *)tmp)->_nextFree;
  if (((t_metadata *)tmp)->_nextFree != end)
    ((t_metadata *)tmp)->_nextFree->_prevFree = ptr;
  ((t_metadata *)tmp)->_nextFree = ptr;
  ((t_metadata *)ptr)->_prevFree = tmp;
  ((t_metadata *)ptr)->_nextFree = ((t_metadata *)tmp)->_nextFree;
#ifdef DEBUG
  show_alloc_mem();
#endif
}

/*void	*realloc(void *ptr, size_t size)
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
  }*/

void	show_alloc_mem()
{
  void		*ptr = start;

  printf("break : %p\n", end);
  while (ptr != end)
    {
      printf("%s", GET_VALUE(((t_metadata *)ptr)->_properties, _USED) ? "\033[31m" : "\033[32m");
      printf("%p - %p : %zu", ptr + sizeof(t_metadata),
	     ptr + ((t_metadata *)ptr)->_allocSize,
	     ((t_metadata *)ptr)->_allocSize - sizeof(t_metadata));
      ptr += ((t_metadata *)ptr)->_allocSize;
      printf("\033[37m\n");
    }
}
