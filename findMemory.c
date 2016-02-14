/*
** findMemory.c for  in /home/resse_e/malloc
**
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
**
** Started on  Sat Feb  6 12:24:08 2016 Enzo Resse
** Last update Sun Feb 14 18:08:20 2016 Maxime Agor
*/

#include "my_malloc.h"

void		*findMemory(void *start, void *end, size_t size)
{
  void		*ptr;
  void		*memPosition;
  void		*last;
  size_t	memory;

  memPosition = 0;
  last = 0;
  memory = 0;
  ptr = ((t_metadata *)start)->_nextFree;
  while (ptr < end && memory < size + (2 * sizeof(t_metadata)))
    mergeMemory(&memPosition, &memory, &ptr, &last);
  if (last != memPosition)
    {
      ((t_metadata *)memPosition)->_nextFree = ((t_metadata *)last)->_nextFree;
      ((t_metadata *)memPosition)->_nextElem = ((t_metadata *)last)->_nextElem;
    }
  ((t_metadata *)memPosition)->_allocSize = memory;
  return (memPosition);
}

inline void	mergeMemory(void **memPosition, size_t *memory,
			    void **ptr, void **last)
{
  if (*memPosition + *memory == *ptr)
    {
      *last = *memPosition + *memory;
      *memory += ((t_metadata *)*ptr)->_allocSize;
    }
  else
    {
      *memPosition = *ptr;
      *memory = ((t_metadata *)*ptr)->_allocSize;
      *last = *ptr;
    }
  *ptr = ((t_metadata *)*ptr)->_nextFree;
}

void		*addMemory(void **end, void *ptr, size_t size)
{
  size_t	space;
  void		*breakPoint;

  if (((t_metadata *)ptr)->_allocSize >= size + (2 * sizeof(t_metadata)))
    return (ptr);
  else
    {
      breakPoint = sbrk(0);
      if (breakPoint != *end)
	{
	  space = getpagesize() *
	    ((size + 2 * sizeof(t_metadata)) / getpagesize() + 1);
	  if (sbrk(space) == (void *) -1)
	    return (0);
	  jumpMemory(ptr, breakPoint, space);
	  ptr = breakPoint;
	}
      else
	{
	  space = getpagesize() * (((size + (2 * sizeof(t_metadata)) -
	       ((t_metadata *)ptr)->_allocSize)) / getpagesize() + 1);
	  if (sbrk(space) == (void *) -1)
	    return (0);
	  ((t_metadata *)ptr)->_allocSize += space;
	  ((t_metadata *)ptr)->_nextFree = sbrk(0);
	  ((t_metadata *)ptr)->_nextElem = sbrk(0);
	}
      *end = sbrk(0);
    }
  return (ptr);
}

void	jumpMemory(void *ptr, void *breakPoint, size_t space)
{
  ((t_metadata *)ptr)->_nextFree = breakPoint;
  ((t_metadata *)ptr)->_nextElem = breakPoint;
  ((t_metadata *)breakPoint)->_allocSize = space;
  ((t_metadata *)breakPoint)->_prevFree = ptr;
  ((t_metadata *)breakPoint)->_nextFree = sbrk(0);
  ((t_metadata *)breakPoint)->_nextElem = sbrk(0);
  ((t_metadata *)breakPoint)->_properties = 0;
  SET_VALUE(((t_metadata *)breakPoint)->_properties, _JUMPED, 1);
}

void		useMemory(void *ptr, size_t size)
{
  size_t	nextData;
  void		*tmp;

  nextData = ((t_metadata *)ptr)->_allocSize;

  ((t_metadata *)ptr)->_allocSize = (size + sizeof(t_metadata));
  SET_VALUE(((t_metadata *)ptr)->_properties, _USED, 1);

  if (size + sizeof(t_metadata) == nextData)
    {
      if (((t_metadata *)ptr)->_nextFree != sbrk(0))
	((t_metadata *)ptr)->_nextFree->_prevFree = ((t_metadata *)ptr)->_prevFree;
      ((t_metadata *)ptr)->_prevFree->_nextFree =
	((t_metadata *)ptr)->_nextFree;
    }
  else
    {
      tmp = ptr + size + sizeof(t_metadata);
      if (((t_metadata *)ptr)->_nextFree != sbrk(0))
	((t_metadata *)ptr)->_nextFree->_prevFree = tmp;
      create_link(&tmp, &ptr, size, nextData);
    }
}
