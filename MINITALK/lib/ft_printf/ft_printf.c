/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:49:16 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/04/20 14:50:25 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	print_format(va_list args, char format)
{
	int	len;

	len = 0;
	if (format == 'c')
		len = ft_char(va_arg(args, int));
	else if (format == 's')
		len = ft_str(va_arg(args, char *));
	else if (format == 'd' || format == 'i')
		len = ft_putnbr(va_arg(args, long int));
	else if (format == 'u')
		len = ft_putunbr(va_arg(args, unsigned int));
	else if ((format == 'x') || (format == 'X'))
		len = ft_hex(va_arg(args, unsigned int), format);
	else if (format == '%')
		len = ft_char('%');
	else if (format == 'p')
		len += ft_ptradress(va_arg(args, unsigned long int));
	return (len);
}

int	ft_printf(const char *form_spec, ...)
{
	va_list	ap;
	int		count;
	int		i;

	count = 0;
	i = 0;
	va_start(ap, form_spec);
	while (form_spec[i] != '\0')
	{
		if (form_spec[i] == '%')
		{
			if (form_spec[i + 1] != '\0')
				count += print_format(ap, form_spec[++i]);
		}
		else
			count += ft_char(form_spec[i]);
		i++;
	}
	va_end(ap);
	return (count);
}

// int	main(void)
// {
// 	char	*str;
// 	int		num;

// 	str = "string of chars";
// 	num = 15;
// 	printf("testing custom prinf, %c, %d\n", str, num);
// 	ft_printf("testing custom prinf, %c, %d\n", str, num);
// 	return (0);
// }
