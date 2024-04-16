
    int ft_hex(unsigned int num, char format)
    {
        int len;
    
        len = 0;
        if (num >= 16)
        {
            len += ft_hex(num / 16, format);
            len+= ft_hex(num % 16, format);
        }
        else if (num <= 9 )
            len += ft_putchar(num + '0');
        else
        {
            if (format == 'x')
                len += ft_putchar(num -10 + 'a');
            if (format == 'X')
                len += ft_putchar(num -10 + 'A');
        }
        return (len);
    }