/*
** my_malloc.c for  in /home/gandoulf/epitech/PSU_project/test/malloc_test
**
** Made by gandoulf
** Login   <gandoulf@epitech.net>
**
** Started on  Tue Jan 26 15:12:19 2016 gandoulf
** Last update Mon Feb  1 16:59:18 2016 gandoulf
*/

#include "my_malloc.h"

static void	*start = NULL;
static void	*end = NULL;

void	*malloc(size_t size)
{
  void	*ptr;
  if (start == NULL)
    {
      start = sbrk(0);
      end = start;
    }
  #ifdef DEBUG
  my_putstr("start = %p, end = %p\n", start, end);
  my_putstr("size of malloc = %d\n", size);
  #endif
  ptr = findMemory(start, end, size);
  #ifdef DEBUG
  my_putstr("\nafter findMemory ptr = %p\n", ptr);
  #endif
  if (addMemory(start, &end, &ptr, size) == 0)
    return (0);
  #ifdef DEBUG
  my_putstr("\nafter addMemory ptr = %p\n", ptr);
  my_putstr("after addMemory start = %p, end = %p\n", start, end);
  #endif
  useMemory(ptr, end, size);
  ptr += sizeof(t_metadata);
  #ifdef DEBUG
  my_putstr("\n\n");
  #endif
  return(ptr);
}

// TODO: set errno
void	free(void *ptr)
{
  if (ptr < start || ptr > end)
    return ;
  ptr -= sizeof(int);
  *(int *)ptr = 0;
}

void	*realloc(void *ptr, size_t size)
{
  if (ptr < start || ptr > end)
    return (malloc(size));
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

  #ifdef DEBUG
  my_putstr("break : %p\n", end);
  #endif
  while (ptr != end)
    {
      data = ptr;
      my_putstr("%p - %p : %d\n", ptr + sizeof(t_metadata), ptr + data->_allocSize, data->_allocSize - sizeof(t_metadata));
      ptr += data->_allocSize;
    }
}
