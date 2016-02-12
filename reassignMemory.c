/*
** reassignMemory.c for  in /home/resse_e/rendu/malloc/newMalloc
** 
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
** 
** Started on  Sat Feb  6 15:23:29 2016 Enzo Resse
** Last update Fri Feb 12 15:23:48 2016 Enzo Resse
*/

#include "my_malloc.h"

void		*increaseMemory(void *ptr, size_t size,
				void *start, void **end)
{
  void		*save = ptr;
  size_t	i = 0;
  size_t	memorySize;

  memorySize = ((t_metadata *)save)->_allocSize - sizeof(t_metadata);
  ((t_metadata *)ptr)->_used = 0;
  ptr = findMemory(start, *end, size);
  if ((ptr = addMemory(end, ptr, size)) == 0)
    return (0);
  useMemory(ptr, size);
  if (ptr != save)
    {
      /*printf("ptr = %p\nsav = %p\nsiz = %zu\n",
	     ptr + sizeof(t_metadata),
	     save + sizeof(t_metadata), 
	     ((t_metadata *)save)->_allocSize - sizeof(t_metadata));*/
      //memcpy(ptr + sizeof(t_metadata), save + sizeof(t_metadata),
      //	     ((t_metadata *)save)->_allocSize - sizeof(t_metadata));
      while (i < memorySize)
	{
	  ((char *)ptr)[i + sizeof(t_metadata)] =
	    ((char *)save)[i + sizeof(t_metadata)];
	  ++i;
	}
    }
  return (ptr);
}

void		reduceMemory(void *ptr, size_t size)
{
  size_t	data;

  data = ((t_metadata *)ptr)->_allocSize;
  ((t_metadata *)ptr)->_allocSize = size + sizeof(t_metadata);
  ptr += ((t_metadata *)ptr)->_allocSize;
  ((t_metadata *)ptr)->_allocSize = data - (size + sizeof(t_metadata));
  ((t_metadata *)ptr)->_used = 0;
}
