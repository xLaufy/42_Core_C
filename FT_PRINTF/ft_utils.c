int ft_putchar(char c)
{
    write(1, &c, 1);
    return (1);
}

int ft_putstr(char *str)
{
    int i;

    if (str == NULL)
    {
        return (write(1, "(null)", 6));
    }
    i = 0;
    while (str[i] != '\0')
    {
        ft_putchar(str[i]);
        i++;
    }
    return (i);
}

int ft_putnbr(int nb)
{
    if (nb < 0)
    {
        ft_putchar ('-');
        nb *= -1;
    }
    if (nb > 9)
    {
        ft_putnbr (nb / 10);
        ft_putchar (nb % 10 + '0');
    }
    else 
    {
        ft_putchar (nb + '0');
    }
    return (nb);
}