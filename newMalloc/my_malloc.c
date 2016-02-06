/*
** my_malloc.c for  in /home/resse_e/malloc
** 
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
** 
** Started on  Sat Feb  6 12:20:25 2016 Enzo Resse
** Last update Sat Feb  6 12:50:57 2016 Maxime Agor
*/

#include "my_malloc.h"

static void	*start = NULL;
static void	*end = NULL;

void    *malloc(size_t size)
{
  void  *ptr;

  if (!size)
    return NULL;
  if (start == NULL)
    {
      start = sbrk(0);
      sbrk(getpagesize());
      ((t_metadata *)start)->_allocSize = getpagesize();
      ((t_metadata *)start)->_used = 0;
      end = sbrk(0);
    }
#ifdef DEBUG
  printf("start = %p, end = %p\n", start, end);
  printf("size of malloc = %d\n", size);
#endif
  size += (size % sizeof(int)) ? sizeof(int) - (size % sizeof(int)) : 0;
  ptr = findMemory(start, end, size);
#ifdef DEBUG
  printf("\nafter findMemory ptr = %p\n", ptr);
#endif
  if (addMemory(start, &end, &ptr, size) == 0)
    return (0);
  /*#ifdef DEBUG
    printf("\nafter addMemory ptr = %p\n", ptr);
    printf("after addMemory start = %p, end = %p\n", start, end);
    #endif
    useMemory(ptr, end, size);
    ptr += sizeof(t_metadata);
    #ifdef DEBUG
    printf("\n\n");
    #endif*/
  return(ptr);
}
