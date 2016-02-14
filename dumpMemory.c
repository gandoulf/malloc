/*
** reassignMemory.c for  in /home/resse_e/rendu/malloc/newMalloc
** 
** Made by Enzo Resse
** Login   <resse_e@epitech.net>
** 
** Started on  Sat Feb  6 15:23:29 2016 Enzo Resse
** Last update Sun Feb 14 13:42:28 2016 Maxime Agor
*/

#include "my_malloc.h"

void	dump_memory(void *start, void *end, void *new, void *old, size_t sizeOld)
{
  void		*ptr = start;
  size_t	colorNew = 0;
  size_t	colorOld = 0;

  printf("\n");
  while (ptr < end)
    {
      if (ptr == new)
	{
	  colorNew = ((t_metadata *)new)->_allocSize;
	  printf("\033[31m");
	}
      if (ptr == old)
	{
	  colorOld = sizeOld;
	  if (!colorNew)
	    printf("\033[32m");
	  else
	    printf("\033[33m");
	}
      printf("%x%x ", *((unsigned char *)(ptr)) / 16, *((unsigned char *)(ptr)) % 16);

      if (colorOld > 0)
	{
	  --colorOld;
	  if (!colorOld)
	    printf("\033[37m");
	  else {
	    --colorNew;
	    if (!colorNew)
	      printf("\033[32m");
	  }
	}
      else if (colorNew > 0)
	--colorNew;

      ++ptr;
    }
  printf("\n");
}
