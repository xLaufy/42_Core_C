/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 16:58:50 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/05/06 16:48:41 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char *box;
	char		*line_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line_read = ft_calloc(sizeof(char), BUFFER_SIZE + 1);
	bytes_read = read(fd, line_read, BUFFER_SIZE);
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
