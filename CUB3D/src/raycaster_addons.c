/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_addons.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 15:23:54 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/22 18:42:22 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	draw_vertical_line(t_cast *cast, t_game *g)
{
	cast->y = cast->draw_start;
	while (cast->y <= cast->draw_end)
	{
		cast->tex_y = (int)cast->tex_pos & (cast->tex->h - 1);
		cast->tex_pos += cast->step;
		cast->c = texel_at(cast->tex, cast->tex_x, cast->tex_y);
		if (cast->side == 1)
			cast->c = ((cast->c & 0xFEFEFE) >> 1);
		put_pixel(cast->x, cast->y, cast->c, g);
		cast->y++;
	}
}

float	calc_prep_dist(t_cast *cast)
{
	if (cast->side == 0)
		return ((cast->map_x - cast->pos_x + (1 - cast->step_x) / 2)
			/ cast->ray_dx);
	else
		return ((cast->map_y - cast->pos_y + (1 - cast->step_y) / 2)
			/ cast->ray_dy);
}

void	calc_tex_x(t_cast *cast)
{
	cast->tex_x = (int)(cast->wallx * (float)cast->tex->w);
	if (cast->side == 0 && cast->ray_dx > 0.0f)
		cast->tex_x = cast->tex->w - cast->tex_x - 1;
	if (cast->side == 1 && cast->ray_dy < 0.0f)
		cast->tex_x = cast->tex->w - cast->tex_x - 1;
}
