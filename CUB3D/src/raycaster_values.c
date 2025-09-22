/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_values.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 20:12:11 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/22 18:40:20 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	init_start_values(t_cast *cast, t_game *g)
{
	cast->fov = 60.0f * PI_VAL / 180.0f;
	cast->dir0 = g->pl.dir - cast->fov / 2.0f;
	cast->step_ang = cast->fov / (float)W_WIDTH;
	cast->pos_x = g->pl.x / (float)TILE;
	cast->pos_y = g->pl.y / (float)TILE;
	cast->x = 0;
}

void	delta_dist(t_cast *cast, t_game *g)
{
	if (cast->ray_dx == 0.0f)
		cast->delta_distx = 1e30f;
	else
		cast->delta_distx = fabsf(1.0f / cast->ray_dx);
	if (cast->ray_dy == 0.0f)
		cast->delta_disty = 1e30f;
	else
		cast->delta_disty = fabsf(1.0f / cast->ray_dy);
}

void	init_steps(t_cast *cast)
{
	if (cast->ray_dx < 0.0f)
		cast->step_x = -1;
	else
		cast->step_x = 1;
	if (cast->ray_dy < 0.0f)
		cast->step_y = -1;
	else
		cast->step_y = 1;
}

void	side_dist_acount(t_cast *cast)
{
	if (cast->ray_dx < 0.0f)
		cast->side_distx = cast->pos_xfrac * cast->delta_distx;
	else
		cast->side_distx = (1.0f - cast->pos_xfrac) * cast->delta_distx;
	if (cast->ray_dy < 0.0f)
		cast->side_disty = cast->pos_yfrac * cast->delta_disty;
	else
		cast->side_disty = (1.0f - cast->pos_yfrac) * cast->delta_disty;
}
