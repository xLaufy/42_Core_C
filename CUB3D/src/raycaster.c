/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 22:12:46 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/21 19:25:17 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	init_start_values(t_cast *cast, t_game *g)
{
	cast->fov = 60.0f * PI_VAL / 180.0f;
	cast->dir0 = g->pl.dir - cast->fov / 2.0f;
	cast->step_ang = cast->fov / (float)W_WIDTH;
	cast->posX = g->pl.x / (float)TILE;
	cast->posY = g->pl.y / (float)TILE;
	cast->x = 0;
}

void	init_other_values(t_cast *cast, t_game *g)
{
	cast->ray_ang = cast->dir0 + cast->step_ang * (float)cast->x;
	cast->ray_dx = cosf(cast->ray_ang);
	cast->ray_dy = sinf(cast->ray_ang);
	cast->mapX = (int)cast->posX;
	cast->mapY = (int)cast->posY;
	cast->deltaDistX = (cast->ray_dx == 0.0f) ? 1e30f : fabsf(1.0f / cast->ray_dx);
	cast->deltaDistY = (cast->ray_dy == 0.0f) ? 1e30f : fabsf(1.0f / cast->ray_dy);
	cast->stepX = (cast->ray_dx < 0.0f) ? -1 : 1;
	cast->stepY = (cast->ray_dy < 0.0f) ? -1 : 1;
	cast->posXfrac = cast->posX - floorf(cast->posX);
	cast->posYfrac = cast->posY - floorf(cast->posY);
	if (cast->ray_dx < 0.0f)
		cast->sideDistX = cast->posXfrac * cast->deltaDistX;
	else
		cast->sideDistX = (1.0f - cast->posXfrac) * cast->deltaDistX;
	if (cast->ray_dy < 0.0f)
		cast->sideDistY = cast->posYfrac * cast->deltaDistY;
	else
		cast->sideDistY = (1.0f - cast->posYfrac) * cast->deltaDistY;
}

void	dda_algorithm(t_cast *cast, t_game *g)
{
	while (1)
	{
		if (cast->sideDistX < cast->sideDistY)
		{
			cast->sideDistX += cast->deltaDistX;
			cast->mapX += cast->stepX;
			cast->side = 0;
		}
		else
		{
			cast->sideDistY += cast->deltaDistY;
			cast->mapY += cast->stepY;
			cast->side = 1;
		}
		if (is_wall(&g->sc, cast->mapX, cast->mapY))
			break ;
	}
}

void	cast_and_draw_all(t_game *g)
{
	t_cast	cast;

	ft_bzero(&cast, 0);
	init_start_values(&cast, g);
	while (cast.x < W_WIDTH)
	{
		init_other_values(&cast, g);
		cast.side = -1;
		dda_algorithm(&cast, g);
		cast.perp = calc_prep_dist(&cast);
		calc_line_height_and_bounds(&cast);
		calc_wallx(&cast);
		select_texture(&cast, g);
		calc_texX(&cast);
		calc_tex_step_and_pos(&cast);
		draw_vertical_line(&cast, g);
		cast.x++;
	}
}
