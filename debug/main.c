#include "my_malloc.h"

int	main()
{
  char	*str;
  char	*str2;
  int	i = 0;
  int	a = 0;

  str = malloc(56);
  for (; i < 56; ++i)
    str[i] = 'a';
  str[55] = 0;
  str2 = malloc(56);
  for (i = 0; i < 56; ++i)
    str2[i] = 'b';
  str2[55] = 0;
  printf("str = %s, str2 = %s\n", str, str2);
  str = realloc(str, 58);
  printf("str = %p\n", str);
  printf("str = %s, str2 = %s\n", str, str2);
  str = malloc(14);
  printf("str = %s, str2 = %s\n", str, str2);
  //show_alloc_mem();
  /*  my_free(str);
  str = my_malloc(52);
  for (i = 0; i < 52; ++i)
    str[i] = 'c';
  str[51] = 0;
  printf("str = %s, str2 = %s\n", str, str2);
  printf("for free str = %p\n", str);
  my_free(str);
  str = my_malloc(56);
  for (i = 0; i < 56; ++i)
    str[i] = 'd';
  str[55] = 0;
  printf("str = %s, str2 = %s\n", str, str2);
  str = my_malloc(14);
  str = my_malloc(90);*/
  return (1);
}
