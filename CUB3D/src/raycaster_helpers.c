/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:15:25 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/21 15:15:53 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	is_wall(t_scene *sc, int mx, int my)
{
	if (mx < 0 || my < 0 || mx >= sc->w || my >= sc->h)
		return (1);
	return (sc->map[my][mx] == '1');
}

unsigned int	texel_at(t_texture *t, int tx, int ty)
{
	int	i;

	if (tx < 0)
		tx = 0;
	if (tx >= t->w)
		tx = t->w - 1;
	if (ty < 0)
		ty = 0;
	if (ty >= t->h)
		ty = t->h - 1;
	i = ty * t->line_len + tx * (t->bpp / 8);
	return ((unsigned char)t->data[i] | ((unsigned char)t->data[i
			+ 1] << 8) | ((unsigned char)t->data[i + 2] << 16));
}
