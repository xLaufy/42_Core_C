/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 18:58:26 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/20 20:11:19 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	i = 0;
	if (!s || start >= ft_strlen((char *)s))
	{
		sub = malloc(1 * sizeof(char));
		sub[0] = "\0";
		return (sub);
	}
	sub = malloc(sizeof(char) * len + 1);
	if (sub == NULL)
		return (NULL);
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}
/*
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	const char		*originalString = "Hello, World!";
	unsigned int	start;
	size_t			length;
	char			*substring;

	start = 7;
	length = 5;
	// Test ft_substr function
	substring = ft_substr(originalString, start, length);
	if (substring != NULL)
	{
		printf("Original String: %s\n", originalString);
		printf("Substring: %s\n", substring);
		// Remember to free the allocated memory
		free(substring);
	}
	else
	{
		printf("Memory allocation or index out of bounds.\n");
	}
	return (0);
}
*/
/*
Description :
 Allocates (with malloc(3)) and returns a substring from the string ’s’.
The substring begins at index ’start’ and is of maximum size ’len’.

*/
