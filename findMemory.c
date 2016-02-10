/*
** findMemory.c for  in /home/resse_e/malloc
**
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
**
** Started on  Sat Feb  6 12:24:08 2016 Enzo Resse
** Last update Wed Feb 10 16:09:17 2016 Enzo Resse
*/

#include "my_malloc.h"

void		*findMemory(void *start, void *end, size_t size)
{
  void		*ptr;
  void		*memPosition = 0;
  void		*last = 0;
  size_t	memory = 0;

  ptr = ((t_metadata *)start)->_nextFree;
  while (ptr < end && memory < size + (2 * sizeof(t_metadata)))
    {
#ifdef DEBUG
      printf("in find memory : ptr = %p\n", ptr);
      printf("in find memory : ptr->next = %p\n", ((t_metadata *)ptr)->_nextFree);
#endif
      if (memPosition + memory == ptr)
	{
	  last = memPosition + memory;
	  memory += ((t_metadata *)ptr)->_allocSize;
	}
      else
	{
	  memPosition = ptr;
	  memory = ((t_metadata *)ptr)->_allocSize;
	  last = ptr;
	}
      ptr = ((t_metadata *)ptr)->_nextFree;
    }
  if (last != memPosition)
    {
      ((t_metadata *)memPosition)->_nextFree = ((t_metadata *)last)->_nextFree;
      ((t_metadata *)memPosition)->_nextElem = ((t_metadata *)last)->_nextElem;
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
#ifdef DEBUG
      printf("INCREASE HEAP !!!");
#endif
      breakPoint = sbrk(0);				// new
#ifdef DEBUG
      printf("end = %p, breakPoint = %p\n", *end, breakPoint);
#endif
      if (breakPoint != *end)				// new
	{
	  space = getpagesize() * (size / getpagesize() + 1); //new
#ifdef DEBUG
	  printf("need a jump\n");
#endif
	  if (sbrk(space) == (void *) -1)		// new
	    return (0);					// new
	  jumpMemory(ptr, breakPoint, space);		// new
	  ptr = breakPoint;				// new
	  //show_alloc_mem();
	}
      else
	{
#ifdef DEBUG
	  printf("no interferance\n");
#endif
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
#ifdef DEBUG
  printf("breakPoint = %p\n", breakPoint);
#endif
  ((t_metadata *)ptr)->_nextFree = breakPoint;
  ((t_metadata *)ptr)->_nextElem = breakPoint;
  ((t_metadata *)breakPoint)->_allocSize = space;
  ((t_metadata *)breakPoint)->_prevFree = ptr;
  ((t_metadata *)breakPoint)->_nextFree = sbrk(0);
  ((t_metadata *)breakPoint)->_nextElem = sbrk(0);
  SET_VALUE(((t_metadata *)breakPoint)->_properties, _USED, 0);
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
#ifdef DEBUG
      printf("puteee !!!\n");
#endif
      if (((t_metadata *)ptr)->_nextFree != sbrk(0))
	((t_metadata *)ptr)->_nextFree->_prevFree = ((t_metadata *)ptr)->_prevFree;
      ((t_metadata *)ptr)->_prevFree->_nextFree = ((t_metadata *)ptr)->_nextFree;
    }
  else
    {
#ifdef DEBUG
      printf("useMemory size != nextData\n");
#endif
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
#ifdef DEBUG
      printf("end of use memory\n");
#endif
    }
}
