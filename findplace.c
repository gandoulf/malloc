/*
** findplace.c for  in /home/gandoulf/epitech/PSU_project/test/malloc_test
**
** Made by gandoulf
** Login   <gandoulf@epitech.net>
**
** Started on  Tue Jan 26 16:38:39 2016 gandoulf
** Last update Mon Feb  1 15:43:35 2016 gandoulf
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
#ifdef DEBUG
      printf("In findMemory data = %p\n", data);
      printf("Propriete from data _allocSize = %zu\n", data->_allocSize);
#endif
      if (data->_used == 0)
	{
	  if (sizeSpace == 0)
	    usableSpace = data;
	  sizeSpace += data->_allocSize - sizeof(t_metadata);
#ifdef DEBUG
	  printf("pute !!!!!\n");
#endif
	}
      else
	{
	  usableSpace = 0;
	  sizeSpace = 0;
	}
#ifdef DEBUG
      printf("In findMemory usableSpace = %p\n", usableSpace);
#endif
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
      memset(*ptr, 0, sizeof(t_metadata));
#ifdef DEBUG
      printf("\nptr = 0: PAGE SIZE ADDED = %zu\n", getpagesize() * ((size + sizeof(t_metadata)) / getpagesize() + 1));
#endif
    }
  else if ((space = *end - *ptr - sizeof(t_metadata) * 2) >= size)
    return (sbrkCheck);
  else
    {
      if ((sbrkCheck = sbrk(getpagesize() * ((size + sizeof(t_metadata) - space) / getpagesize() + 1))) == (void *) -1)
	return (0);
#ifdef DEBUG
      printf("\nPAGE SIZE ADDED = %zu\n", getpagesize() * ((size + sizeof(t_metadata)) / getpagesize() + 1));
#endif
    }
  *end = sbrk(0);
  return (sbrkCheck);
}

void		useMemory(void *ptr, void *end, size_t size)
{
  t_metadata	*data = ptr;
  t_metadata	nextData;


  memcpy(&nextData, data, sizeof(t_metadata));
  //nextData._allocSize = giveMemory(ptr, size);
  nextData._allocSize = ((t_metadata *)ptr)->_allocSize;
#ifdef DEBUG
  printf("nextData allocsize = %zu, used = %d\n", nextData._allocSize, nextData._used);
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
  if (nextData._allocSize == 0)
    {
      data->_allocSize = end - (void *)data;
      data->_used = 0;
    }
  else if (nextData._allocSize != (size + sizeof(t_metadata)))
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
