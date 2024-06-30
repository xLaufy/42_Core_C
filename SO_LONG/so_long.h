#ifndef SO_LONG_H
# define SO_LONG_H

# include <sys/time.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <X11/X.h>
# include <X11/keysym.h>

# include "lib/libft/libft.h"


# include "minilibx-linux/mlx.h"

void *mlx_xpm_file_to_image(void *mlx_ptr, char *filename, int *width, int *height);

# endif