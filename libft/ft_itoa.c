/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 14:52:17 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/17 18:32:17 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_num(long num)
{
	int	count;

	count = 0;
	if (num < 0)
	{
		count++;
		num *= -1;
	}
	else if (num == 0)
		count++;
	while (num > 0)
	{
		num /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	long	num_count;
	char	*result;

	num_count = count_num(n);
	result = malloc((num_count + 1) * sizeof(char));
	if (n == 0)
		result[0] = 48;
	if (result == NULL)
		return (NULL);
	if (n < 0)
	{
		result[0] = '-';
		n *= -1;
	}
	result[num_count] = '\0';
	while ((num_count - 1) >= 0 && n > 0)
	{
		result[num_count - 1] = (n % 10) + 48;
		n = (unsigned int)n / 10;
		num_count--;
	}
	return (result);
}

/*
int	main(void)
{
	int	num;

	num = -1234567;
	printf("num is equal to %i \n", count_num(num));
	printf("num is equal to %s \n", ft_itoa(num));
	return (0);
}
*/
