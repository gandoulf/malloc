/*
** findMemory.c for  in /home/resse_e/malloc
** 
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
** 
** Started on  Sat Feb  6 12:24:08 2016 Enzo Resse
** Last update Mon Feb  8 15:29:32 2016 Enzo Resse
*/

#include "my_malloc.h"

void		*findMemory(void *start, void *end, size_t size)
{
  void		*ptr = start;
  void		*memPosition = 0;
  size_t	memory = 0;

  while (ptr < end && memory < size + (2 * sizeof(t_metadata)))
    {
      if (((t_metadata *)ptr)->_allocSize == 0)
	return (0);
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
  ((t_metadata *)memPosition)->_allocSize = memory;
  return (memPosition);
}
 
void		*addMemory(void **end, void *ptr, size_t size)
{
  size_t	space;

  if (ptr == 0 || ptr >= *end)
    {
#ifdef DEBUG
      printf("FATAL ERROR ON PTR!!!\nptr = %p end = %p\n", ptr, *end);
#endif
      return (0);
    }

  space = getpagesize() * (((size + (2 * sizeof(t_metadata)) - ((t_metadata *)ptr)->_allocSize)) / getpagesize() + 1);

  if (((t_metadata *)ptr)->_allocSize >= size + (2 * sizeof(t_metadata)))
    return (ptr);
  else
    {
      if (sbrk(space) == (void *) -1)
	return (0);
      ((t_metadata *)ptr)->_allocSize += space;
      //*end = sbrk(0);
      *end += space;
    }
  return (ptr);
}

void		useMemory(void *ptr, size_t size)
{
  size_t	nextData;

  nextData = ((t_metadata *)ptr)->_allocSize;
  ((t_metadata *)ptr)->_allocSize = (size + sizeof(t_metadata));
  ((t_metadata *)ptr)->_used = 1;
  ptr += (size + sizeof(t_metadata));
  if (nextData != size + sizeof(t_metadata))
    {
      ((t_metadata *)ptr)->_allocSize = nextData - (size + sizeof(t_metadata));
      ((t_metadata *)ptr)->_used = 0;
    }
}