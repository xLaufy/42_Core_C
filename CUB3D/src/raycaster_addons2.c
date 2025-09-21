/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_addons2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 15:33:57 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/21 15:34:28 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	select_texture(t_cast *cast, t_game *g)
{
	if (cast->side == 0 && cast->stepX < 0)
		cast->tex = &g->tex_we;
	else if (cast->side == 0)
		cast->tex = &g->tex_ea;
	else if (cast->side == 1 && cast->stepY < 0)
		cast->tex = &g->tex_no;
	else
		cast->tex = &g->tex_so;
}

void	calc_tex_step_and_pos(t_cast *cast)
{
	cast->step = (float)cast->tex->h / (float)cast->line_h;
	cast->texPos = (cast->draw_start - (W_HEIGHT / 2 - cast->line_h / 2))
		* cast->step;
}
void	calc_line_height_and_bounds(t_cast *cast)
{
	cast->line_h = (int)(W_HEIGHT / cast->perp);
	cast->draw_start = -cast->line_h / 2 + W_HEIGHT / 2;
	cast->draw_end = cast->line_h / 2 + W_HEIGHT / 2;
	if (cast->draw_start < 0)
		cast->draw_start = 0;
	if (cast->draw_end >= W_HEIGHT)
		cast->draw_end = W_HEIGHT - 1;
}

void	calc_wallx(t_cast *cast)
{
	if (cast->side == 0)
		cast->wallx = cast->posY + cast->perp * cast->ray_dy;
	else
		cast->wallx = cast->posX + cast->perp * cast->ray_dx;
	cast->wallx -= floorf(cast->wallx);
}
