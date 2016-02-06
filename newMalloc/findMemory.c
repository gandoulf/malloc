/*
** findMemory.c for  in /home/resse_e/malloc
** 
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
** 
** Started on  Sat Feb  6 12:24:08 2016 Enzo Resse
** Last update Sat Feb  6 13:30:38 2016 Enzo Resse
*/

#include "my_malloc.h"

void		*findMemory(void *start, void *end, size_t size)
{
  void		*ptr = start;
  void		*memPosition = 0;
  size_t	memory = 0;

  while (ptr < end && memory < size + (2 * sizeof(t_metadata)))
    {
      if (((t_metadata *)ptr)->_used == 0)
	{
	  if (memPosition == 0)
	    memPosition = ptr;
	  memory += ((t_metadata *)ptr)->_allocSize;
	}
      else
	{
	  memPosition = 0;
	  memory = 0;
	}
      ptr += ((t_metadata *)ptr)->_allocSize;
    }
  printf("at the end of findMemoryend = %p, ptr = %p\n", end, ptr); 
  ((t_metadata *)memPosition)->_allocSize = memory;
  return (memPosition);
}
 
void		*addMemory(void **end, void *ptr, size_t size)
{
  if (ptr == 0 || ptr >= *end)
    printf("FATAL ERROR ON PTR!!!\nptr = %p end = %p\n", ptr, *end);

  if (((t_metadata *)ptr)->_allocSize >= size + (2 * sizeof(t_metadata)))
    return (ptr);
  else
    {
      if (sbrk(getpagesize() * ((size + (2 * sizeof(t_metadata)) - ((t_metadata *)ptr)->_allocSize) / getpagesize() + 1)) == (void *) -1)
	return (0);
    }
}

