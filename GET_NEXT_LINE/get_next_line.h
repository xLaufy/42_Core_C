
#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

#  define BUFFER_SIZE 10

void	*ft_calloc(size_t nmemb, size_t size);
char *read_the_fcking_file(char *box, int fd);
char	*get_next_line(int fd);
void	ft_bzero(void *s, size_t n);
size_t	ft_strlen(const char *str);
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char *box, char *read_buffer);

# endif
