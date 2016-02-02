/*
** resignedMemory.c for  in /home/gandoulf/epitech/PSU_project/test/malloc_test/debug
**
** Made by gandoulf
** Login   <gandoulf@epitech.net>
**
** Started on  Thu Jan 28 10:11:23 2016 gandoulf
** Last update Tue Feb  2 16:33:40 2016 gandoulf
*/

#include "my_malloc.h"

void	*increasMemory(void **ptr, size_t size, void *start, void *end)
{
  t_metadata	*data = *ptr;

  data->_used = 0;
  *ptr = findMemory(*ptr, end, size);
  if (addMemory(start, &end, ptr, size) == 0)
    return (0); //sbrk error change function
  useMemory(*ptr, end, size);
  if ((void *)data != ptr)
    memcpy(*ptr + sizeof(t_metadata), data + 1, data->_allocSize - sizeof(t_metadata));
  return (start);
}

void	reducedMemory(void *ptr, size_t size)
{
  t_metadata	data;
  t_metadata	*nData;

  if (size <= (*(size_t *)(ptr) - 2 * sizeof(t_metadata)))
    {
      memcpy(&data, ptr, sizeof(t_metadata));
      *(size_t *)(ptr) = size + sizeof(t_metadata);
      ptr += *(size_t *)(ptr);
      nData = ptr;
      nData->_allocSize = data._allocSize - (size + sizeof(t_metadata));
      nData->_used = 0;
    }
}
