// #include "get_next_line.h"

char	*get_the_line(fd)
{
	char	*line_read;
	int		bytes_read;

	line_read = malloc(sizeof(char) * BUFFER_SIZE + 1);
	bytes_read = read(fd, line_read, BUFFER_SIZE);
	if (line_read == 0 || bytes_read <= 0)
	{
		free(line_read);
		return (NULL);
	}
	line_read[bytes_read] = '\0';
	return (line_read);
}

char	*fill_the_buffer(int fd, char *line, char *buffer)
{
}
