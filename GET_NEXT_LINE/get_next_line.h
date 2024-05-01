
#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

#  define BUFFER_SIZE 10

char	*get_next_line(int fd);

# endif
