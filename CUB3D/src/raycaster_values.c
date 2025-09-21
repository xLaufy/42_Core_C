/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_values.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 20:12:11 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/21 20:12:39 by rkobelie         ###   ########.fr       */
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

void	delta_dist(t_cast *cast, t_game *g)
{
	if (cast->ray_dx == 0.0f)
		cast->deltaDistX = 1e30f;
	else
		cast->deltaDistX = fabsf(1.0f / cast->ray_dx);
	if (cast->ray_dy == 0.0f)
		cast->deltaDistY = 1e30f;
	else
		cast->deltaDistY = fabsf(1.0f / cast->ray_dy);
}

void	init_steps(t_cast *cast)
{
	if (cast->ray_dx < 0.0f)
		cast->stepX = -1;
	else
		cast->stepX = 1;
	if (cast->ray_dy < 0.0f)
		cast->stepY = -1;
	else
		cast->stepY = 1;
}

void	side_dist_acount(t_cast *cast)
{
	if (cast->ray_dx < 0.0f)
		cast->sideDistX = cast->posXfrac * cast->deltaDistX;
	else
		cast->sideDistX = (1.0f - cast->posXfrac) * cast->deltaDistX;
	if (cast->ray_dy < 0.0f)
		cast->sideDistY = cast->posYfrac * cast->deltaDistY;
	else
		cast->sideDistY = (1.0f - cast->posYfrac) * cast->deltaDistY;
}
