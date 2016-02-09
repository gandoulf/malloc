/*
** findMemory.c for  in /home/resse_e/malloc
**
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
**
** Started on  Sat Feb  6 12:24:08 2016 Enzo Resse
** Last update Tue Feb  9 13:57:12 2016 gandoulf
*/

#include "my_malloc.h"

void		*findMemory(void *start, void *end, size_t size)
{
  void		*ptr;
  void		*memPosition = 0;
  size_t	memory = 0;

  ptr = ((t_metadata *)start)->_nextFree;
  while (ptr < end && memory < size + (2 * sizeof(t_metadata)))
    {
      printf("in find memory : ptr = %p\n", ptr);
      printf("in find memory : ptr->next = %p\n", ((t_metadata *)ptr)->_nextFree);
      if (memPosition + memory == ptr)
	memory += ((t_metadata *)ptr)->_allocSize;
      else
	{
	  memPosition = ptr;
	  memory = ((t_metadata *)ptr)->_allocSize;
	}
      ptr = ((t_metadata *)ptr)->_nextFree;
    }
  ((t_metadata *)memPosition)->_allocSize = memory;
  return (memPosition);
}

void		*addMemory(void **end, void *ptr, size_t size)
{
  size_t	space;
  void		*breakPoint;				// new

  if (((t_metadata *)ptr)->_allocSize >= size + (2 * sizeof(t_metadata)))
    return (ptr);
  else
    {
      printf("INCREASE HEAP !!!");
      breakPoint = sbrk(0);				// new
      if (breakPoint != *end)				// new
	{
	  space = getpagesize() * (size / getpagesize() + 1); //new
	  if (sbrk(space) == (void *) -1)		// new
	    return (0);					// new
	  jumpMemory(ptr, breakPoint, space);		// new
	  ptr = breakPoint;				// new
	}
      else
	{
	  space = getpagesize() * (((size + (2 * sizeof(t_metadata)) - ((t_metadata *)ptr)->_allocSize)) / getpagesize() + 1);
	  if (sbrk(space) == (void *) -1)
	    return (0);
	  ((t_metadata *)ptr)->_allocSize += space;
	  ((t_metadata *)ptr)->_nextFree = sbrk(0);	// new
	  ((t_metadata *)ptr)->_nextElem = sbrk(0);	// new
	}
      *end = sbrk(0);
    }
  return (ptr);
}

void	jumpMemory(void *ptr, void *breakPoint, size_t space)
{
  void	*jumpMyFriend;

  jumpMyFriend = breakPoint;
  ((t_metadata *)ptr)->_nextFree = breakPoint;
  ((t_metadata *)ptr)->_nextElem = breakPoint;
  ((t_metadata *)jumpMyFriend)->_allocSize = space;
  ((t_metadata *)jumpMyFriend)->_prevFree = ptr;
  ((t_metadata *)jumpMyFriend)->_nextFree = sbrk(0);
  ((t_metadata *)jumpMyFriend)->_nextElem = sbrk(0);
  SET_VALUE(((t_metadata *)jumpMyFriend)->_properties, _USED, 0);
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
      ((t_metadata *)ptr)->_prevFree->_nextFree = ((t_metadata *)ptr)->_nextFree;
    }
  else
    {
      printf("useMemory size != nextData\n");
      tmp = ptr + size + sizeof(t_metadata);
      if (((t_metadata *)ptr)->_nextFree != sbrk(0))
	((t_metadata *)ptr)->_nextFree->_prevFree = tmp;
      ((t_metadata *)ptr)->_prevFree->_nextFree = tmp;
      ((t_metadata *)tmp)->_allocSize = nextData - (size + sizeof(t_metadata));
      ((t_metadata *)tmp)->_nextFree = ((t_metadata *)ptr)->_nextFree;
      ((t_metadata *)tmp)->_prevFree = ((t_metadata *)ptr)->_prevFree;
      ((t_metadata *)tmp)->_nextElem = ((t_metadata *)ptr)->_nextElem;
      SET_VALUE(((t_metadata *)tmp)->_properties, _USED, 0);
      ((t_metadata *)ptr)->_nextElem = tmp;
    }
}
