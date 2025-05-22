/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:05:01 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/05/22 18:12:21 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	ft_strcmp(const char *str1, const char *str2)
{
	size_t			i;
	unsigned char	*s1;
	unsigned char	*s2;

	s1 = (unsigned char *)str1;
	s2 = (unsigned char *)str2;
	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static int	check_values(char **argv, int argc)
{
	int	i;

	if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[2]) < 1 || ft_atoi(argv[3]) < 1
		|| ft_atoi(argv[4]) < 1)
	{
		printf("Wrong argument.\n");
		return (1);
	}
	if (argc == 6 && ft_atoi(argv[5]) < 1)
	{
		printf("Program doesn't acceppt values below 1.\n");
		return (1);
	}
	i = 1;
	while (argv[i])
	{
		if (ft_atoi(argv[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	basic_argcheck(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc > 6 || argc < 5)
	{
		printf("Wrong number of arguments.\n");
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf("Program accepts only positive numbers as arguments.\n");
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	check_argv(int argc, char **argv)
{
	if (basic_argcheck(argc, argv))
		return (1);
	if (ft_atoi(argv[1]) > 200)
	{
		printf("Too much Philosophers, try between 1-200\n");
		return (1);
	}
	if (check_values(argv, argc) == 1)
		return (1);
	return (0);
}
