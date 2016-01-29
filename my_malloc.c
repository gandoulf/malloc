/*
** my_malloc.c for  in /home/gandoulf/epitech/PSU_project/test/malloc_test
**
** Made by gandoulf
** Login   <gandoulf@epitech.net>
**
** Started on  Tue Jan 26 15:12:19 2016 gandoulf
** Last update Fri Jan 29 13:43:06 2016 gandoulf
*/

#include "my_malloc.h"

static void	*start;
static void	*end;
static int	a = 1;

void	*malloc(size_t size)
{
  void	*ptr;
  if (a)
    {
      start = sbrk(0);
      end = start;
      a = 0;
    }
  #ifdef DEBUG
  printf("start = %p, end = %p\n", start, end);
  #endif
  ptr = findMemory(start, end, size);
  #ifdef DEBUG
  printf("\nafter findMemory ptr = %p\n", ptr);
  #endif
  if (addMemory(start, &end, &ptr, size) == 0)
    return (0);
  #ifdef DEBUG
  printf("\nafter addMemory ptr = %p\n", ptr);
  printf("after addMemory start = %p, end = %p\n", start, end);
  #endif
  useMemory(ptr, end, size);
  ptr += sizeof(t_metadata);
  #ifdef DEBUG
  printf("\n\n");
  #endif
  return(ptr);
}

void	free(void *ptr)
{
  if (ptr < start || ptr > end)
    return ;	//error should be set
  ptr -= sizeof(int);
  *(int *)ptr = 0;
}

void	*realloc(void *ptr, size_t size)
{
  if (ptr < start || ptr > end)
    return (malloc(size));	//should return malloc
  if (size == 0)
    free(ptr);
  ptr -= sizeof(t_metadata);
  if (*(size_t *)ptr - sizeof(t_metadata) < size)
    {
      if (increasMemory(&ptr, size, start, end) == 0)
	return (0);
    }
  else if (*(size_t *)ptr > size)
    reducedMemory(ptr, size);
  ptr += sizeof(t_metadata);
  return (ptr);
}

void	show_alloc_mem()
{
  void		*ptr = start;
  t_metadata	*data;

  printf("break : %p\n", end);
  while (ptr != end)
    {
      data = ptr;
      printf("%p - %p : %zu\n", ptr + sizeof(t_metadata), ptr + data->_allocSize, data->_allocSize - sizeof(t_metadata));
      ptr += data->_allocSize;
    }
}
