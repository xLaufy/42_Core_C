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

char	*take_the_line(char *box)
{
	size_t	i;
	char	*str;

	i = 0;
	if (!box[i])
		return (NULL);
	while (box[i] && box[i] != '\n')
		i++;
	str = (char *)malloc(i + 2);
	if (!str)
		return (NULL);
	i = 0;
	while (box[i] && box[i] != '\n')
	{
		str[i] = box[i];
		i++;
	}
	if (box[i] == '\n')
	{
		str[i] = box[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*remains(char *box)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	while (box[i] && box[i] != '\n')
		i++;
	if (!box[i])
	{
		free(box);
		return (NULL);
	}
	str = (char *)malloc(sizeof(char) * (ft_strlen(box) - i + 1));
	if (!str)
		return (NULL);
	i++;
	j = 0;
	while (box[i])
		str[j++] = box[i++];
	str[j] = '\0';
	free(box);
	return (str);
}

char	*read_the_file(char *box, int fd)
{
	char	*temp_stash;
	ssize_t	bytes_read;

	temp_stash = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (temp_stash == 0)
		return (NULL);
	while ((bytes_read = read(fd, temp_stash, BUFFER_SIZE)) > 0)
	{
		temp_stash[bytes_read] = '\0';
		box = ft_strjoin(box, temp_stash);
		if (ft_strchr(box, '\n'))
			break ;
	}
	free(temp_stash);
	if (bytes_read == -1)
	{
		free(box);
		return (NULL);
	}
	return (box);
}

char	*get_next_line(int fd)
{
	static char	*box;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!box)
		box = ft_strdup("");
	if (!ft_strchr(box, '\n'))
		box = read_the_file(box, fd);
	if (!box)
		return (NULL);
	line = take_the_line(box);
	box = remains(box);
	return (line);
}


int	main(void)
{
	int		fd;
	char	*next_line;
	int		count;

	count = 0;
	fd = open("text.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("Error, can't open the file\n");
		return (1);
	}
	printf("Reading from file text.txt:\n");
	while ((next_line = get_next_line(fd)) !=  NULL)
	{
		count++;
		printf("%d : %s\n", count, next_line);
		free(next_line);
	}
	close(fd);
	return (0);
}
