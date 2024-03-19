/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 18:58:26 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/17 18:58:27 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	const char	*sub;
	size_t		s_len;
	size_t		sub_len;
	size_t		i;

	s_len = 0;
	i = 0;
	if (s == NULL)
		return (NULL);
	while (s[s_len] != '\0')
		s_len++;
	if (start >= s_len)
		return (NULL);
	sub_len = len;
	if (start + len > s_len)
		sub_len = s_len - start;
	sub = malloc((sub_len + 1) * sizeof(char));
	if (sub != NULL)
	{
		while (i < sub_len)
		{
			sub[i] = s[start + i];
			i++;
		}
		sub[i] = '\0';
	}
	return (sub)
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
Parameters :
s: The string from which to create the substring.
start: The start index of the substring in the string ’s’.
len: The maximum length of the substring.

Return value :
The substring.
NULL if the allocation fails.

External functs. malloc

Description :
 Allocates (with malloc(3)) and returns a substring from the string ’s’.
The substring begins at index ’start’ and is of maximum size ’len’.


1: Declare variables to store the substring, length of the input string,
actual length of the substring, and an iterator.

2: Check if the input string is NULL. If it is,return NULL to indicate an error.

3: Calculate the length of the input string by iterating through each character
until the null terminator is encountered.

4: Check if the start index is beyond the length of the string. If it is,
return NULL to indicate an error.

5: Calculate the actual length of the substring based on the provided length
and start index.

6: Allocate memory for the substring, including space for the characters and
the null terminator.

7: Check if memory allocation was successful. If so, copy characters from the
original string to the substring using a while loop and add a null terminator
at the end.

8: Return the pointer to the substring.
*/
