/*
** findplace.c for  in /home/gandoulf/epitech/PSU_project/test/malloc_test
**
** Made by gandoulf
** Login   <gandoulf@epitech.net>
**
** Started on  Tue Jan 26 16:38:39 2016 gandoulf
** Last update Tue Feb  2 16:31:02 2016 gandoulf
*/

#include "my_malloc.h"

void	*findMemory(void *start, void *end, size_t size)
{
  t_metadata	*data = start;
  void		*usableSpace = 0;
  size_t	sizeSpace = 0;
  int		nb = 0;

  while ((void *)data < end && sizeSpace != size && sizeSpace <= size +sizeof(t_metadata))
    {
#ifdef DEBUG
      printf("In findMemory data = %p\n", data);
      printf("Propriete from data _allocSize = %zu\n", data->_allocSize);
#endif
      if (data->_used == 0)
	{
	  if (sizeSpace == 0)
	    usableSpace = data;
	  sizeSpace += data->_allocSize - sizeof(t_metadata);
	  nb += 1;
#ifdef DEBUG
	  printf("pute !!!!!\n");
#endif
	}
      else
	{
	  usableSpace = 0;
	  sizeSpace = 0;
	  nb = 0;
	}
#ifdef DEBUG
      printf("In findMemory usableSpace = %p\n", usableSpace);
#endif
      data = (void *)data + data->_allocSize;
    }
  ((t_metadata *)usableSpace)->_allocSize = sizeSpace + (sizeof(t_metadata) * nb);
  return (usableSpace);
}


void		*addMemory(void *start, void **end, void **ptr, size_t size)
{
  size_t	space = 0;
  void		*sbrkCheck = start;

  if ((space = (*end - *ptr) - (sizeof(t_metadata) * 2)) >= size)
    {
#ifdef DEBUG
      printf ("ptr = %p\nend = %p\n", *ptr, *end);
#endif
      return (sbrkCheck);
    }
  else
    {
      if ((sbrkCheck = sbrk(getpagesize() * ((size + sizeof(t_metadata) - space) / getpagesize() + 1))) == (void *) -1)
	return (0);
      ((t_metadata *)(*ptr))->_allocSize += getpagesize() * ((size + sizeof(t_metadata)) / getpagesize() + 1);
      *end = sbrk(0);
#ifdef DEBUG
      printf("\nPAGE SIZE ADDED = %zu\n", getpagesize() * ((size + sizeof(t_metadata)) / getpagesize() + 1));
#endif
    }
  return (sbrkCheck);
}

//TODO remove _attribute

void		useMemory(void *ptr, void __attribute__((unused)) *end , size_t size)
{
  t_metadata	*data = ptr;
  t_metadata	nextData;

  memcpy(&nextData, ptr, sizeof(t_metadata));
#ifdef DEBUG
  printf("nextData allocsize = %zu, used = %d of the adresse %p\n", nextData._allocSize, nextData._used, data);
#endif
  data->_allocSize = (size + sizeof(t_metadata));
  data->_used = 1;
#ifdef DEBUG
  printf("In useMemory data alloc on : %p\n", data);
  printf("set the alloc size to : %zu\n", data->_allocSize);
  printf("size = %zu metadata = %zu\n", size, sizeof(t_metadata));
#endif
  data = (void *)data + (size + sizeof(t_metadata));
#ifdef DEBUG
  printf("In useMemory data righted on : %p\n", data);
#endif
  if (nextData._allocSize != (size + sizeof(t_metadata)))
    {
      data->_allocSize = nextData._allocSize - (size + sizeof(t_metadata));
      data->_used = 0;
    }
#ifdef DEBUG
  printf("set the end of memory to : %zu\n", data->_allocSize);
#endif
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
