/*
** my_malloc.c for  in /home/resse_e/malloc
** 
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
** 
** Started on  Sat Feb  6 12:20:25 2016 Enzo Resse
** Last update Sat Feb  6 15:54:28 2016 Enzo Resse
*/

#include "my_malloc.h"

static void     *start = NULL;
static void     *end = NULL;

void    *malloc(size_t size)
{
  void  *ptr;
  if (start == NULL)
    {
      start = sbrk(0);
      sbrk(getpagesize());
      ((t_metadata *)start)->_allocSize = getpagesize();
      ((t_metadata *)start)->_used = 0;
      end = sbrk(0);
    }
#ifdef DEBUG
  printf("start = %p, end = %p\n", start, end);
  printf("size of malloc = %zu\n", size);
#endif
  size += (size % sizeof(int)) ? sizeof(int) - (size % sizeof(int)) : 0;
  ptr = findMemory(start, end, size);
#ifdef DEBUG
  printf("\nafter findMemory ptr = %p\n", ptr);
#endif
  if (addMemory(&end, ptr, size) == 0)
    return (0);
#ifdef DEBUG
  printf("\nafter addMemory ptr = %p\n", ptr);
  printf("after addMemory start = %p, end = %p\n", start, end);
#endif
  useMemory(ptr, size);
  ptr += sizeof(t_metadata);
#ifdef DEBUG
  printf("\n\n");
#endif
  return(ptr);
}

void	free(void *ptr)
{
  if (ptr < start + sizeof(t_metadata) || ptr >= end)
    return;
  ptr -= sizeof(t_metadata);
  ((t_metadata *)ptr)->_used = 0;
}

void	*realloc(void *ptr, size_t size)
{
  if (ptr < start || ptr > end)
    return (malloc(size));
  if (size == 0)
    free(ptr);
  ptr -= sizeof(t_metadata);
  size += (size % sizeof(int)) ? sizeof(int) - (size % sizeof(int)) : 0;
  if (((t_metadata *)ptr)->_allocSize < size + sizeof(t_metadata))
    {
      if ((ptr = increasMemory(ptr, size, start, &end)) == 0)
	return (0);
    }
  else if (size <= ((t_metadata *)ptr)->_allocSize - 2 * sizeof(t_metadata))
    reducedMemory(ptr, size);
  ptr += sizeof(t_metadata);
  return (ptr);
}

void	show_alloc_mem()
{
  void		*ptr = start;

  printf("break : %p\n", end);
  while (ptr != end)
    {
      printf("%p - %p : %zu\n", ptr + sizeof(t_metadata),
	     ptr + ((t_metadata *)ptr)->_allocSize,
	     ((t_metadata *)ptr)->_allocSize - sizeof(t_metadata));
      ptr += ((t_metadata *)ptr)->_allocSize;
    }
}
