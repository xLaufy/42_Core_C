/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/09/21 10:10:27 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/09/21 10:10:27 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	set_colors(int color1, int color2, double t)
{
	int	r;
	int	g;
	int	b;

	r = (int)((1 - t) * ((color1 >> 16) & 0xFF) + t * ((color2 >> 16) & 0xFF)
			* 5);
	g = (int)((1 - t) * ((color1 >> 8) & 0xFF) + t * ((color2 >> 8) & 0xFF)
			* 5);
	b = (int)((1 - t) * (color1 & 0xFF) + t * (color2 & 0xFF) * 5);
	r = (int)(r + (255 - r) * t * 1.5);
	g = (int)(g + (255 - g) * t * 0.2);
	b = (int)(b + (255 - b) * t * 4);
	return ((r << 16) | (g << 8) | b);
}

void	color_shift(t_fractol *fract)
{
	static int	colors[] = {ELECTRIC, LAVA, WHITE, AQUA, PASTELPINK, GREEN,
		LIGHTENING, BLUE, PASTELYELLOW, MAGENTA, LIME, ORANGE,
		PURPLE, PINK, YELLOW};
	static int	color_index;

	color_index = 0;
	color_index = (color_index + 1) % (sizeof(colors) / sizeof(colors[0]));
	fract->current_color = colors[color_index];
	render_fractal(fract);
}
