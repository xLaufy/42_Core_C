/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:51 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/17 19:00:52 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	const char		*s;
	unsigned int	i;

	d = (char *)dest;
	s = src;
	i = 0;
	while (i < n && (dest || src))
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
