/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/09/21 10:11:12 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/09/21 10:11:12 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

static void	set_fractal(t_fractol *z, t_fractol *c, t_fractol *fract)
{
	if (!ft_strncmp(fract->title, "Julia", 5))
	{
		c->cmplx_r = fract->julia_r;
		c->cmplx_i = fract->julia_i;
	}
	else
	{
		c->cmplx_r = z->cmplx_r;
		c->cmplx_i = z->cmplx_i;
	}
}

static void	ft_pixel_put(int x, int y, t_fractol *fract, int color)
{
	int	offset;

	offset = (y * fract->img_line + x * (fract->img_bpp / 8));
	*(unsigned int *)(fract->img_addr + offset) = color;
}

static void	get_complex_map(int x, int y, t_fractol *fract)
{
	t_fractol	z;
	t_fractol	c;
	int			i;
	int			rgb;

	i = 0;
	z.cmplx_r = (map((t_map_coords){x, -2, +2, 0, WIDTH}) * fract->zoom)
		+ fract->shift_r;
	z.cmplx_i = (map((t_map_coords){y, +2, -2, 0, HEIGHT}) * fract->zoom)
		+ fract->shift_i;
	set_fractal(&z, &c, fract);
	while (i < fract->iterations)
	{
		z = sum_complex(square_complex(z), c);
		if ((z.cmplx_r * z.cmplx_r) + (z.cmplx_i
				* z.cmplx_i) > fract->hypotenuse)
		{
			rgb = set_colors(BLACK, fract->current_color, (double)i
					/ fract->iterations);
			ft_pixel_put(x, y, fract, rgb);
			return ;
		}
		++i;
	}
	ft_pixel_put(x, y, fract, BLACK);
}

void	render_fractal(t_fractol *fract)
{
	int	x;
	int	y;

	y = -1;
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
			get_complex_map(x, y, fract);
	}
	mlx_put_image_to_window(fract->mlx_connect,
		fract->window,
		fract->img,
		0,
		0);
}
