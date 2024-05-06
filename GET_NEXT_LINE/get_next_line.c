/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 16:58:50 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/05/01 13:25:32 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*dest;

	i = 0;
	dest = s;
	while (i < n)
		dest[i++] = 0;
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*tmp;

	if (nmemb == 0 || size == 0)
	{
		nmemb = 1;
		size = 1;
	}
	if (2147483647 / nmemb < size)
		return (NULL);
	tmp = malloc(nmemb * size);
	if (!tmp)
		return (NULL);
	ft_bzero(tmp, nmemb * size);
	return (tmp);
}

char	*get_next_line(int fd)
{
	static char *box;
	char		*line_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line_read = ft_calloc(sizeof(char), BUFFER_SIZE + 1);
	if (line_read == 0 || bytes_read <= 0)
	{
		free(line_read);
		return (NULL);
	}
	line_read[bytes_read] = '\0';
	return (line_read);
}

int	main(void)
{
	int		fd;
	char	*next_line;
	int		count;

	count = 0;
	fd = open("text.txt", O_RDONLY);
	if (fd < 0)
	{
		printf("Error, can't open the file\n");
		return (1);
	}
	printf("Reading from file text.txt:\n");
	while (1)
	{
		next_line = get_next_line(fd);
		if (next_line == NULL)
			break ;
		count++;
		printf("%d : %s\n", count, next_line);
		free(next_line);
	}
	close(fd);
	return (0);
}
