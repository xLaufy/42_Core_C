/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/09/21 10:19:43 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/09/21 10:19:43 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "../lib/ft_printf/ft_printf.h"
# include "../lib/libft/libft.h"
# include "../minilibx-linux/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>

# define WIDTH 1000
# define HEIGHT 1000

# define BLACK 0x000000
# define MAGENTA 0xFF00FF
# define GREEN 0x00FF00
# define WHITE 0xFFFFFF
# define LIME 0xCCFF00
# define BLUE 0x0000FF
# define PURPLE 0x9932CC
# define AQUA 0x33CCCC
# define LIGHTENING 0x00CCFF
# define PASTELYELLOW 0xFFFF99
# define PINK 0xFF66B2
# define ELECTRIC 0x0066FF
# define ORANGE 0xFF6600
# define LAVA 0xFF3300
# define YELLOW 0xFFFF00
# define PASTELPINK 0xFFB6C1

typedef struct s_fractol
{
	char	*title;
	void	*mlx_connect;
	void	*window;
	void	*img;
	char	*img_addr;
	int		img_bpp;
	int		img_line;
	int		img_endian;
	double	cmplx_r;
	double	cmplx_i;
	double	hypotenuse;
	int		iterations;
	double	shift_r;
	double	shift_i;
	double	zoom;
	int		current_color;
	double	julia_r;
	double	julia_i;
}			t_fractol;

typedef struct s_map_coords
{
	double	unscaled_num;
	double	new_min;
	double	new_max;
	double	old_min;
	double	old_max;
}			t_map_coords;

void		fractol_init(t_fractol *fract);

void		render_fractal(t_fractol *fract);
int			set_colors(int color1, int color2, double t);
void		color_shift(t_fractol *fract);

int			key_handler(int key, t_fractol *fract);
int			handle_mouse(int button, int x, int y, t_fractol *fract);
void		zoom_in(t_fractol *fract, double mouse_r, double mouse_i);
void		zoom_out(t_fractol *fract, double mouse_r, double mouse_i);
int			clean_exit(t_fractol *fract);
void		instructions(void);

double		map(t_map_coords coords);
t_fractol	sum_complex(t_fractol z1, t_fractol z2);
t_fractol	square_complex(t_fractol z);
void		malloc_error(void);
double		atod(char *s);

#endif
