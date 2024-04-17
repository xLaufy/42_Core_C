/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 20:22:20 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/04/17 20:22:21 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putunbr(unsigned long int num)
{
	unsigned int	len;

	len = 0;
	if (num > 9)
	{
		len += ft_putunbr(num / 10);
		len += ft_putunbr(num % 10);
	}
	else
		len += ft_putchar(num + '0');
	return (len);
}
