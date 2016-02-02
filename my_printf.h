/*
** my_printf.h for  in /home/agor_m/RenduTek1/printf
**
** Made by Maxime Agor
** Login   <agor_m@epitech.net>
**
** Started on  Tue Nov 11 18:33:03 2014 Maxime Agor
** Last update Sun Nov 16 19:45:50 2014 Maxime Agor
*/

#ifndef MY_PRINTF_
# define MY_PRINTF_
//# include "../my.h"

int	my_printf(const char *format, ...);
int	my_putuint_base(long nb, char *base);
int	print_percent(va_list args);
int	print_char(va_list args);
int	print_int(va_list args);
int	print_pointer(va_list args);
int	print_str(va_list args);
int	print_str_oct(va_list args);
int	print_uint_oct(va_list args);
int	print_uint_dec(va_list args);
int	print_uint_hex(va_list args);
int	print_uint_hexup(va_list args);
int	print_uint_bin(va_list args);

#endif
