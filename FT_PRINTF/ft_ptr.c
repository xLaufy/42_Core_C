/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ptr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 20:22:16 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/04/17 20:22:17 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_ptradress(unsigned long int num)
{
	int	len;

	len = 0;
	if (num >= 16)
	{
		len += ft_ptradress(num / 16);
		len += ft_ptradress(num % 16);
	}
	else if (num <= 9)
		len += ft_putchar(num + '0');
	else
		len += ft_putchar(num - 10 + 'a');
	return (len);
}
