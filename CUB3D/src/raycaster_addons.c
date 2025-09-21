/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_addons.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 15:23:54 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/21 15:34:30 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"


void	draw_vertical_line(t_cast *cast, t_game *g)
{
	cast->y = cast->draw_start;
	while (cast->y <= cast->draw_end)
	{
		cast->texY = (int)cast->texPos & (cast->tex->h - 1);
		cast->texPos += cast->step;
		cast->c = texel_at(cast->tex, cast->texX, cast->texY);
		if (cast->side == 1)
			cast->c = ((cast->c & 0xFEFEFE) >> 1);
		put_pixel(cast->x, cast->y, cast->c, g);
		cast->y++;
	}
}

float	calc_prep_dist(t_cast *cast)
{
	if (cast->side == 0)
		return ((cast->mapX - cast->posX + (1 - cast->stepX) / 2)
			/ cast->ray_dx);
	else
		return ((cast->mapY - cast->posY + (1 - cast->stepY) / 2)
			/ cast->ray_dy);
}

void	calc_texX(t_cast *cast)
{
	cast->texX = (int)(cast->wallx * (float)cast->tex->w);
	if (cast->side == 0 && cast->ray_dx > 0.0f)
		cast->texX = cast->tex->w - cast->texX - 1;
	if (cast->side == 1 && cast->ray_dy < 0.0f)
		cast->texX = cast->tex->w - cast->texX - 1;
}

