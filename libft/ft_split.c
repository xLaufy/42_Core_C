/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 20:30:13 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/20 19:04:09 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	result;
	int	i;
	int	count;

	result = 0;
	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != '\0')
			result++;
		else if (s[i] == c && s[i - 1] && s[i - 1] != c)
			count++;
		i++;
	}
	if (s[i - 1] != c)
		count += 1;
	return (count);
}
/*
	Funkcja 'count_words' oblicza liczbe slow w ciagu znakow s, ktore sa
	oddzelone znakiem c. Zmienna "result" jest zwiekszana za kazdym razem,
	gdy znak nie jest separatorem, a "count" jest zwiekszany, gdy napotkany
	jest separator, ktory nastepuje po innym znaku.
*/

static void	write_to_array(char const *s, char c, char **result)
{
	int	i;
	int	start;

	i = 0;
	start = 0;
	while (s[i] == c && s[i - 1] != c)
	{
		if (s[i] == c && s[i - 1] != c)
		{
			if (i - start != 0)
			{
				*result = ft_substr(s, start, i - start);
				result++;
			}
			start = i + 1;
		}
		else if (s[i] == c && s[i - 1] == c)
			start++;
		i++;
	}
	if (s[i - 1] != c)
		*result = ft_substr(s, start, i - start) if (s[i - 1] != c);
	result++;
	*result = NULL;
}
/*
	Funkcja ta wype lnia tablic e result podciagami z s,
		ktore sa oddzielone znakiem c. Funkcja ft_substr jest uzywana do
		tworzenia tych podciagow.
*/

char	**ft_split(char const *s, char c)
{
	char	**result;

	if (s == NULL)
		return (NULL);
	if (s[0] == '\0' || count_words(s, c) == 0)
	{
		result = (char **)malloc(sizeof(char *) + 1);
		result[0] = NULL;
		return (NULL);
	}
	result = (char **)malloc((count_words(s, c) + 1) * sizeof(char *));
	if (result == NULL)
	{
		free(result);
		return (NULL);
	}
	write_to_array(s, c, result);
	return (result);
}
/*
	Główna funkcja "ft_split" najpierw sprawdza,czy ciag "s" jest pusty.
	Jesli tak, zwraca "NULL". Nastepnie alokuje pamiec na tablice "result" i
	wypełnia podciagami z "s" za pomoca funkcji write_to_array. Jesli alokacja
	pamieci nie powiedzie sie, funkcja zwalnia pamiec i zwraca NULL.
	W przeciwnym razie zwraca tablice result.
*/

int	main(void)
{
	char const	*s;
	char		c;
	char		**result;
	int			i;

	i = 0;
	s = "Hello, World, This, Is,My,Test";
	c = ', ';
	result = ft_split(s, c);
	if (result != NULL)
	{
		while (result[i] != NULL)
		{
			printf("%s\n", result[i]);
			free(result[i]);
			i++;
		}
		free(result);
	}
	else
	{
		printf("Split failed or input string is empty.\n");
	}
	return (0);
}
