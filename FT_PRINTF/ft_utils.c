/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 20:22:23 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/04/20 14:14:13 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (write(1, "(null)", 6), 6);
	while (str[i])
		i++;
	return (write(1, str, i), i);
}

int	ft_putnbr(int nb)
{
	long	i;
	int		len;

	i = nb;
	len = 0;
	if (i < 0)
	{
		i = -i;
		len += ft_putchar('-');
	}
	if (i > 9)
	{
		len += ft_putnbr(i / 10);
		len += ft_putchar((i % 10) + '0');
	}
	else
		len += ft_putchar(i + '0');
	return (len);
}
