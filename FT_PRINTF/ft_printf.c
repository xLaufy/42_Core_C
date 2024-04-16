#include <stdio.h>
#include <stdarg.h>

void ft_printf(const char* format, ...)
{
    int count;
    int i;

    count = 0;
    i = 0;
    va_list args;
    va_start(args, format);
    if (format == NULL)
    {
        return (-1);
    }
    while (format[i] != '\0')
    {
        if (format[i] == '%')
        {
            if (format[i + 1] != '\0')
            count += form_spec(va_args, format[i + 1]);
        }
        else 
            count += ft_putchar(format[i]);
        i++;
    }
    va_end(args);
    return (count)
}
 int form_spec(va_list args, char format)
 {
    int count;

    count = 0;
    if (format == 'c')
        count += ft_putchar(va_args(args, char))
    else if (format == 's')
        count += ft_putchar(va_args(args, char *))
    else if (format == 'd' || format == 'i')
        count += ft_putnbr(va_args(args, int))
    else if (format == 'u')
        count += ft_putnbr(va_args(args, unsigned int))
    else if (format == 'x' || format == 'X'){
        count += ft_hex(va_args(args, unsigned int), format)
    else if (format == '%')
        count += ft_putchar('%')
    return (count);
    }        
 }

int main()
{
    printf("Hello, %s!\n", "world");
    ft_printf("Hello, %s!\n", "world");
    printf("The answer is %d.\n", 42);
    ft_printf("The answer is %d.\n", 42);
    
    return 0;
}
