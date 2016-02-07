/*
** findMemory.c for  in /home/resse_e/malloc
** 
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
** 
** Started on  Sat Feb  6 12:24:08 2016 Enzo Resse
** Last update Sun Feb  7 13:07:09 2016 Enzo Resse
*/

#include "my_malloc.h"

void		*findMemory(void *start, void *end, size_t size)
{
  void		*ptr = start;
  void		*memPosition = 0;
  size_t	memory = 0;

  while (ptr < end && memory < size + (2 * sizeof(t_metadata)))
    {
#ifdef DEBUG
      printf("In findMemory data = %p\n", ptr);
      printf("Propriete from data _allocSize = %zu\n", ((t_metadata *)ptr)->_allocSize);
#endif
      if (((t_metadata *)ptr)->_allocSize == 0)
	return (0);
      if (((t_metadata *)ptr)->_used == 0)
	{
	  if (memPosition == 0)
	    memPosition = ptr;
	  memory += ((t_metadata *)ptr)->_allocSize;
#ifdef DEBUG
          printf("pute !!!!!\n");
#endif
	}
      else
	{
	  memPosition = 0;
	  memory = 0;
	}
#ifdef DEBUG
      printf("In findMemory usableSpace = %p\n", memPosition);
#endif
      ptr += ((t_metadata *)ptr)->_allocSize;
    }
#ifdef DEBUG
  printf("at the end of findMemoryend = %p, ptr = %p\n", end, ptr); 
#endif
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

#ifdef DEBUG
  printf("space = %zu, size = %zu\n", space, size);
  printf ("ptr = %p\nend = %p\n", ptr, *end);
#endif

  if (((t_metadata *)ptr)->_allocSize >= size + (2 * sizeof(t_metadata)))
    return (ptr);
  else
    {
      // space = getpagesize() * ((size + (2 * sizeof(t_metadata)) - ((t_metadata *)ptr)->_allocSize) / getpagesize() + 1);
      if (sbrk(space) == (void *) -1)
	return (0);
      ((t_metadata *)ptr)->_allocSize += space;
      //      printf ("end = %p\n", *end);
      *end += space;
      //*end = sbrk(0);
      //#ifdef DEBUG
      //printf("PAGE SIZE ADDED = %zu\n",space);

      //#endif
    }
  return (ptr);
}

void		useMemory(void *ptr, size_t size)
{
  size_t	nextData;

  nextData = ((t_metadata *)ptr)->_allocSize;
#ifdef DEBUG
  printf("nextData allocsize = %zu of the adresse %p\n", nextData, ptr);
#endif
  ((t_metadata *)ptr)->_allocSize = (size + sizeof(t_metadata));
  ((t_metadata *)ptr)->_used = 1;
#ifdef DEBUG
  printf("In useMemory data alloc on : %p\n", ptr);
  printf("set the alloc size to : %zu\n", ((t_metadata *)ptr)->_allocSize);
  printf("size = %zu metadata = %zu\n", size, sizeof(t_metadata));
#endif
  ptr += (size + sizeof(t_metadata));
#ifdef DEBUG
  printf("In useMemory data righted on : %p\n", ptr);
#endif
  if (nextData != size + sizeof(t_metadata))
    {
      ((t_metadata *)ptr)->_allocSize = nextData - (size + sizeof(t_metadata));
      ((t_metadata *)ptr)->_used = 0;
    }
#ifdef DEBUG
  printf("set the end of memory to : %zu\n", ((t_metadata *)ptr)->_allocSize);
#endif
}
