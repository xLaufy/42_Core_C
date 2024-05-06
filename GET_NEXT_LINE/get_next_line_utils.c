#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

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
	while (*s)
	{
		if (s == char(c))
			return ((char *)s);
		s++;
		*s = '\0'
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_strjoin(char *box, char *read_buffer)
{
	char	*temp;
	int		i;
	int		j;

	if (!box || !read_buffer)
		return (NULL);
	temp = malloc((ft_strlen(box) + ft_strlen(read_buffer) + 1) * sizeof(char));
	if (!temp)
		return (NULL);
	while (box[i])
		temp[j++] = box[i++];
	i = 0;
	while (read_buffer[i])
		temp[j++] = read_buffer[i++];
	temp[j] = '\0';
	return (temp);
}


// ta funkcja czyta plik tekstowy i zapisuje ciaag znakow o dlugosci BUFFER_SIZE az nie napotka '\n' albo '\0', nastepnie zapisuje go do tablicy temp_stash.

char *read_the_fcking_file(char *box, int fd)
{
	char *temp_stash;
	size_t	bytes_read;

	temp_stash = ft_calloc(sizeof(char), BUFFER_SIZE + 1);
	if (temp_stash == 0)
		return (NULL);
	bytes_read = read(fd, temp_stash, BUFFER_SIZE);
	if (bytes_read <= 0)
		return (free(bytes_read), NULL);
	temp_stash[bytes_read] = '\0';
	box = ft_strjoin(box, temp_stash);
	if (bytes_read == -1)
		free(box);
	if (ft_strchr(box, '\n') || ft_strchr(box, '\0'))
		break;
	free(temp_stash);
	return (box);
}





























// Funkcja pomocnicza do podziału bufora na linie
char	*split_line(char **buffer, int i)
{
	char	*line;

	line = (char *)malloc((i + 1) * sizeof(char)); // Alokacja pamięci dla linii
	line[i] = '\0';                                // Dodanie znaku końca linii
	while (--i >= 0)
		// Kopiowanie znaków z bufora do linii
	{
		line[i] = (*buffer)[i];
		(*buffer)[i] = '\0';
	}
	*buffer += i + 1; // Przesunięcie wskaźnika bufora
	return (line);    // Zwrócenie linii
}

// Główna funkcja get_next_line
char	*get_next_line(int fd)
{
	char		*line;
	static char	*buffer;

	// Statyczny bufor do przechowywania nieprzetworzonych danych
	int bytes_read, i;
	if (!buffer)
		buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	// Alokacja pamięci dla bufora
	bytes_read = read(fd, buffer, BUFFER_SIZE); // Czytanie z pliku
	if (bytes_read <= 0)
		return (NULL);         // Jeśli nie ma więcej danych do przeczytania,
			zwróć NULL
	buffer[bytes_read] = '\0'; // Dodanie znaku końca linii do bufora
	i = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
		// Szukanie znaku nowej linii w buforze
		i++;
	if (buffer[i] == '\n') // Jeśli znaleziono znak nowej linii
	{
		line = split_line(&buffer, i); // Podziel bufor na linie
		return (line);                 // Zwróć linię
	}
	else                            // Jeśli nie znaleziono znaku nowej linii
		return (get_next_line(fd)); // Wywołaj funkcję rekurencyjnie
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
