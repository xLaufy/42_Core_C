/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 22:12:46 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/21 22:02:54 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	init_other_values(t_cast *cast, t_game *g)
{
	cast->ray_ang = cast->dir0 + cast->step_ang * (float)cast->x;
	cast->ray_dx = cosf(cast->ray_ang);
	cast->ray_dy = sinf(cast->ray_ang);
	cast->mapX = (int)cast->posX;
	cast->mapY = (int)cast->posY;
	delta_dist(cast, g);
	init_steps(cast);
	cast->posXfrac = cast->posX - floorf(cast->posX);
	cast->posYfrac = cast->posY - floorf(cast->posY);
	side_dist_acount(cast);
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
		calc_tex_x(&cast);
		calc_tex_step_and_pos(&cast);
		draw_vertical_line(&cast, g);
		cast.x++;
	}
}
