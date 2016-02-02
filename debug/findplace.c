/*
** findplace.c for  in /home/gandoulf/epitech/PSU_project/test/malloc_test
**
** Made by gandoulf
** Login   <gandoulf@epitech.net>
**
** Started on  Tue Jan 26 16:38:39 2016 gandoulf
** Last update Tue Feb  2 10:55:27 2016 gandoulf
*/

#include "my_malloc.h"

void	*findMemory(void *start, void *end, size_t size)
{
  t_metadata	*data = start;
  void		*usableSpace = 0;
  size_t	sizeSpace = 0;

  if (start == end)
    return (0);
  while ((void *)data < end && sizeSpace != size && sizeSpace <= size +sizeof(t_metadata))
    {
      if (data->_used == 0)
	{
	  if (sizeSpace == 0)
	    usableSpace = data;
	  sizeSpace += data->_allocSize - sizeof(t_metadata);
	}
      else
	{
	  usableSpace = 0;
	  sizeSpace = 0;
	}
      data = (void *)data + data->_allocSize;
    }
  ((t_metadata *)usableSpace)->_allocSize = sizeSpace;
  return (usableSpace);
}


void		*addMemory(void *start, void **end, void **ptr, size_t size)
{
  size_t	space = 0;
  void		*sbrkCheck = start;

  if (*ptr == 0)
    {
      if ((sbrkCheck = sbrk(getpagesize() * ((size + sizeof(t_metadata)) / getpagesize() + 1))) == 0)
	return (0);
      *ptr = start;
      ((t_metadata *)(*ptr))->_allocSize = getpagesize() * ((size + sizeof(t_metadata)) / getpagesize() + 1);
      ((t_metadata *)(*ptr))->_used = 0;
    }
  else if ((space = (*end - *ptr) - (sizeof(t_metadata) * 2)) >= size)
    {
      return (sbrkCheck);
    }
  else
    {
      if ((sbrkCheck = sbrk(getpagesize() * ((size + sizeof(t_metadata) - space) / getpagesize() + 1))) == (void *) -1)
	return (0);
    }
  *end = sbrk(0);
  return (sbrkCheck);
}

//TODO remove _attribute

void		useMemory(void *ptr, void __attribute__((unused)) *end , size_t size)
{
  t_metadata	*data = ptr;

  data->_allocSize = (size + sizeof(t_metadata));
  data->_used = 1;
  data = (void *)data + (size + sizeof(t_metadata));
  if (((t_metadata *)ptr)->_allocSize != (size + sizeof(t_metadata)))
    {
      data->_allocSize = ((t_metadata *)ptr)->_allocSize - (size + sizeof(t_metadata));
      data->_used = 0;
    }
}

/*size_t		giveMemory(void *ptr, size_t size)
{
  size_t	memory = 0;

  if (*(size_t *)(ptr) == 0)
    return (0);
  while (memory != (size + sizeof(t_metadata)) && memory < (size + 2 * sizeof(t_metadata)))
    {
      memory += *(size_t *)(ptr);
      ptr += *(size_t *)(ptr);
    }
  return (memory);
  }*/
