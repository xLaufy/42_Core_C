/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 20:22:23 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/04/17 20:43:59 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (write(1, str, i), i);
}

int	ft_putnbr(long int n)
{
	long int	i;

	i = n;
	if (n < 0)
	{
		ft_putchar('-');
		i = -1;
	}
	if (i > 9)
	{
		ft_putnbr(i / 10);
		i %= 10;
	}
	return (ft_putchar(i + '0'));
}
