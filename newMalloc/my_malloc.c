/*
** my_malloc.c for  in /home/resse_e/malloc
** 
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
** 
** Started on  Sat Feb  6 12:20:25 2016 Enzo Resse
** Last update Mon Feb  8 13:10:48 2016 Enzo Resse
*/

#include "my_malloc.h"

static void     *start = NULL;
static void     *end = NULL;

void    *malloc(size_t size)
{
  printf("USE MALLOC !!!!! malloc %zu\n", size);
  void  *ptr;

  if (!size)
    return NULL;
  if (start == NULL)
    {
      printf("mymalloc\n");
      start = sbrk(0);
      sbrk(getpagesize());
      ((t_metadata *)start)->_allocSize = getpagesize();
      ((t_metadata *)start)->_used = 0;
      end = sbrk(0);
    }
#ifdef DEBUG
  printf("start = %p, end = %p\n", start, end);
  printf("size of malloc = %zu\n", size);
#endif
  size += (size % sizeof(int)) ? sizeof(int) - (size % sizeof(int)) : 0;
  ptr = findMemory(start, end, size);
#ifdef DEBUG
  printf("\nafter findMemory ptr = %p\n", ptr);
#endif
  if (addMemory(&end, ptr, size) == 0)
    {
      printf("add memory fail\n");
      return (0);
    }
#ifdef DEBUG
  printf("\nafter addMemory ptr = %p\n", ptr);
  printf("after addMemory start = %p, end = %p\n", start, end);
#endif
  useMemory(ptr, size);
  ptr += sizeof(t_metadata);
#ifdef DEBUG
  printf("\n\n");
#endif
  show_alloc_mem();
  return(ptr);
}

void	free(void *ptr)
{
  printf("USE FREE !!!!!, free this : %p\n",ptr);
#ifdef DEBUG
  printf("\nUSE FREE !!!!!\n");
#endif
  if (ptr < start + sizeof(t_metadata) || ptr >= end)
    return;
  ptr -= sizeof(t_metadata);
  ((t_metadata *)ptr)->_used = 0;
  show_alloc_mem();
}

void	*realloc(void *ptr, size_t size)
{
  printf("USE REALLOC !!!!! realloc this : %p, of %zu\n", ptr, size);
#ifdef DEBUG
  printf("\nUSE REALLOC !!!!!\n");
#endif
  if (ptr < start + sizeof(t_metadata) || ptr > end)
    return (malloc(size));
  if (size == 0)
  {
    free(ptr);
    return NULL;
  }
  ptr -= sizeof(t_metadata);
  size += (size % sizeof(int)) ? sizeof(int) - (size % sizeof(int)) : 0;
  if (((t_metadata *)ptr)->_allocSize < size + sizeof(t_metadata))
    {
      if ((ptr = increaseMemory(ptr, size, start, &end)) == 0)
	return (0);
    }
  else if (((t_metadata *)ptr)->_allocSize >= size + 2 * sizeof(t_metadata))
    reduceMemory(ptr, size);
  //else
  //return ptr;
  ptr += sizeof(t_metadata);
  show_alloc_mem();
  return (ptr);
}

void		*calloc(size_t nmemb, size_t size)
{
  void		*ptr;

  printf("USE CALLOC !!!!! nmemb = %zu, size  %zu\n", nmemb, size);
  if (!(ptr = malloc(nmemb * size)))
    return NULL;
  printf("AFTER MALLOC CALL !!!!! ptr = %p\n", ptr);
  memset(ptr, 0, nmemb * size);
  show_alloc_mem();
  return (ptr);  
}

void	show_alloc_mem()
{
  void		*ptr = start;

  printf("end = %p\n", end);
  printf("break : %p\n", end);
  while (ptr != end)
    {
      printf("%s", ((t_metadata *)ptr)->_used ? "\033[31m" : "\033[32m");
      printf("%p - %p : %zu", ptr + sizeof(t_metadata),
	     ptr + ((t_metadata *)ptr)->_allocSize,
	     ((t_metadata *)ptr)->_allocSize - sizeof(t_metadata));
      ptr += ((t_metadata *)ptr)->_allocSize;
      printf("\033[37m\n");
    }
}
