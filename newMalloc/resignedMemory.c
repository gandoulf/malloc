/*
** resignedMemory.c for  in /home/resse_e/rendu/malloc/newMalloc
** 
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
** 
** Started on  Sat Feb  6 15:23:29 2016 Enzo Resse
** Last update Sat Feb  6 16:48:08 2016 Enzo Resse
*/

#include "my_malloc.h"

void	*increasMemory(void *ptr, size_t size, void *start, void **end)
{
  void	*save = ptr;

  ((t_metadata *)ptr)->_used = 0;
  ptr = findMemory(start, *end, size);
  if (addMemory(end, ptr, size) == 0)
    return (0);
  useMemory(ptr, size);
  if (ptr != save)
    memcpy(ptr + sizeof(t_metadata), save + sizeof(t_metadata),
	   ((t_metadata *)save)->_allocSize - sizeof(t_metadata));
  return (ptr);
}

void		reducedMemory(void *ptr, size_t size)
{
  size_t	data;

  data = ((t_metadata *)ptr)->_allocSize;
  ((t_metadata *)ptr)->_allocSize = size + sizeof(t_metadata);
  ptr += data;
  ((t_metadata *)ptr)->_allocSize = data - (size + sizeof(t_metadata));
  ((t_metadata *)ptr)->_used = 0;
}
