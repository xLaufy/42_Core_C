#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	char	ch;

	ch = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == ch)
			return ((char *)&s[i]);
		i++;
	}
	if (ch == '\0')
		return ((char *)&s[i]);
	return (NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	temp = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!temp)
		return (NULL);
	while (s1[i])
		temp[j++] = s1[i++];
	i = 0;
	while (s2[i])
		temp[j++] = s2[i++];
	temp[j] = '\0';
	free((void *)(s1));
	return (temp);
}

char	*ft_strdup(const char *s1)
{
	char	*str;
	int		i;
	int		j;

	j = ft_strlen(s1);
	i = 0;
	str = malloc(sizeof(char) * j + 1);
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*sub;

	i = start;
	j = 0;
	sub = (char *)malloc(len + 1);
	if (!s || !sub)
		return (NULL);
	while (i < ft_strlen(s) && j < len)
		sub[j++] = s[i++];
	sub[j] = '\0';
	return (sub);
}
char	*take_the_line(char *box)
{
	int		i;
	char	*str;

	i = 0;
	if (!box[i])
		return (NULL);
	while (box[i] && box[i] != '\n')
		i++;
	str = (char *)malloc(i + 2);
	if (!str)
		return (NULL);
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
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (box[i] && box[i] != '\n')
		i++;
	if (!box[i])
		return (NULL);
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
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, temp_stash, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(temp_stash), NULL);
		temp_stash[bytes_read] = '\0';
		box = ft_strjoin(box, temp_stash);
		if (ft_strchr(box, '\n') || ft_strchr(box, '\0'))
			break ;
	}
	free(temp_stash);
	return (box);
}

char	*get_next_line(int fd)
{
	static char	*box;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	box = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!ft_strchr(box, '\n') || !ft_strchr(box, '\0'))
		box = read_the_file(box, fd);
	if (!box)
		return (free(box), NULL);
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
