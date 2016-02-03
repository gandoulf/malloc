/*
** my_malloc.c for  in /home/gandoulf/epitech/PSU_project/test/malloc_test
**
** Made by gandoulf
** Login   <gandoulf@epitech.net>
**
** Started on  Tue Jan 26 15:12:19 2016 gandoulf
** Last update Wed Feb  3 09:54:22 2016 gandoulf
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
      sbrk(getpagesize());
      ((t_metadata *)start)->_allocSize = getpagesize();
      ((t_metadata *)start)->_used = 0;
      end = sbrk(0);
    }
  #ifdef DEBUG
  my_printf("start = %p, end = %p\n", start, end);
  my_printf("size of malloc = %d\n", size);
  #endif
  ptr = findMemory(start, end, size);
  #ifdef DEBUG
  my_printf("\nafter findMemory ptr = %p\n", ptr);
  #endif
  if (addMemory(start, &end, &ptr, size) == 0)
    return (0);
  #ifdef DEBUG
  my_printf("\nafter addMemory ptr = %p\n", ptr);
  my_printf("after addMemory start = %p, end = %p\n", start, end);
  #endif
  useMemory(ptr, end, size);
  ptr += sizeof(t_metadata);
  #ifdef DEBUG
  my_printf("\n\n");
  #endif
  return(ptr);
}

// TODO: set errno
void	free(void *ptr)
{
#ifdef DEBUG
  my_printf("FREE USED!!!! for this adresse %p\n", ptr);
#endif
  if (ptr < start || ptr > end)
    return ;
  ptr -= sizeof(int);
  *(int *)ptr = 0;
}

void	*realloc(void *ptr, size_t size)
{
#ifdef DEBUG
  my_printf("REALLOC USED!!!!!\n");
#endif
  if (ptr < start || ptr > end || ptr == NULL)
    return (malloc(size));
  if (size == 0)
    free(ptr);
  ptr -= sizeof(t_metadata);
#ifdef DEBUG
  my_printf("memories asked %d, current memories %d\n", size, ((t_metadata *)ptr)->_allocSize);
#endif
  if (((t_metadata *)ptr)->_allocSize - sizeof(t_metadata) < size)
    {
      if (increasMemory(&ptr, size, start, &end) == 0)
	return (0);
    }
  else if (((t_metadata *)ptr)->_allocSize > size)
    reducedMemory(ptr, size);
  ptr += sizeof(t_metadata);
  return (ptr);
}

void	show_alloc_mem()
{
  void		*ptr = start;
  t_metadata	*data;

  #ifdef DEBUG
  my_printf("break : %p\n", end);
  #endif
  while (ptr != end)
    {
      data = ptr;
      my_printf("%p - %p : %d\n", ptr + sizeof(t_metadata), ptr + data->_allocSize, data->_allocSize - sizeof(t_metadata));
      ptr += data->_allocSize;
    }
}
