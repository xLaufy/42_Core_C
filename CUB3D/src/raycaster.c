/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 22:12:46 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/22 18:40:43 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	init_other_values(t_cast *cast, t_game *g)
{
	cast->ray_ang = cast->dir0 + cast->step_ang * (float)cast->x;
	cast->ray_dx = cosf(cast->ray_ang);
	cast->ray_dy = sinf(cast->ray_ang);
	cast->map_x = (int)cast->pos_x;
	cast->map_y = (int)cast->pos_y;
	delta_dist(cast, g);
	init_steps(cast);
	cast->pos_xfrac = cast->pos_x - floorf(cast->pos_x);
	cast->pos_yfrac = cast->pos_y - floorf(cast->pos_y);
	side_dist_acount(cast);
}

void	dda_algorithm(t_cast *cast, t_game *g)
{
	while (1)
	{
		if (cast->side_distx < cast->side_disty)
		{
			cast->side_distx += cast->delta_distx;
			cast->map_x += cast->step_x;
			cast->side = 0;
		}
		else
		{
			cast->side_disty += cast->delta_disty;
			cast->map_y += cast->step_y;
			cast->side = 1;
		}
		if (is_wall(&g->sc, cast->map_x, cast->map_y))
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
