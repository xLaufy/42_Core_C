/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 22:12:46 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/20 17:52:07 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/cub3d.h"

void	cast_and_draw_all(t_game *g)

{
	t_cast	cast;
	ft_bzero(&cast, 0);
	cast.fov = 60.0f * PI_VAL / 180.0f;
	cast.dir0 = g->pl.dir - cast.fov / 2.0f;
	cast.step_ang = cast.fov / (float)W_WIDTH;
	cast.posX = g->pl.x / (float)TILE;
	cast.posY = g->pl.y / (float)TILE;
	cast.x = 0;
	while (cast.x < W_WIDTH)
	{
		cast.ray_ang = cast.dir0 + cast.step_ang * (float)cast.x;
		cast.ray_dx = cosf(cast.ray_ang);
		cast.ray_dy = sinf(cast.ray_ang);
		cast.mapX = (int)cast.posX;
		cast.mapY = (int)cast.posY;
		cast.deltaDistX = (cast.ray_dx == 0.0f) ? 1e30f : fabsf(1.0f / cast.ray_dx);
		cast.deltaDistY = (cast.ray_dy == 0.0f) ? 1e30f : fabsf(1.0f / cast.ray_dy);
		cast.stepX = (cast.ray_dx < 0.0f) ? -1 : 1;
		cast.stepY = (cast.ray_dy < 0.0f) ? -1 : 1;
		cast.posXfrac = cast.posX - floorf(cast.posX);
		cast.posYfrac = cast.posY - floorf(cast.posY);
		if (cast.ray_dx < 0.0f)
			cast.sideDistX = cast.posXfrac * cast.deltaDistX;
		else
			cast.sideDistX = (1.0f - cast.posXfrac) * cast.deltaDistX;
		if (cast.ray_dy < 0.0f)
			cast.sideDistY = cast.posYfrac * cast.deltaDistY;
		else
			cast.sideDistY = (1.0f - cast.posYfrac) * cast.deltaDistY;
		cast.side = -1;
		while (1)
		{
			if (cast.sideDistX < cast.sideDistY)
			{
				cast.sideDistX += cast.deltaDistX;
				cast.mapX += cast.stepX;
				cast.side = 0;
			}
			else
			{
				cast.sideDistY += cast.deltaDistY;
				cast.mapY += cast.stepY;
				cast.side = 1;
			}
			if (is_wall(&g->sc, cast.mapX, cast.mapY))
				break ;
		}
		if (cast.side == 0)
			cast.perp = (cast.mapX - cast.posX + (cast.stepX < 0 ? 1.0f : 0.0f)) / cast.ray_dx;
		else
			cast.perp = (cast.mapY - cast.posY + (cast.stepY < 0 ? 1.0f : 0.0f)) / cast.ray_dy;
		cast.line_h = (int)(W_HEIGHT / cast.perp);
		cast.draw_start = -cast.line_h / 2 + W_HEIGHT / 2;
		cast.draw_end = cast.line_h / 2 + W_HEIGHT / 2;
		if (cast.draw_start < 0)
			cast.draw_start = 0;
		if (cast.draw_end >= W_HEIGHT)
			cast.draw_end = W_HEIGHT - 1;
		if (cast.side == 0)
			cast.wallx = cast.posY + cast.perp * cast.ray_dy;
		else
			cast.wallx = cast.posX + cast.perp * cast.ray_dx;
		cast.wallx -= floorf(cast.wallx);
		if (cast.side == 0 && cast.stepX < 0)
			cast.tex = &g->tex_we;
		else if (cast.side == 0)
			cast.tex = &g->tex_ea;
		else if (cast.side == 1 && cast.stepY < 0)
			cast.tex = &g->tex_no;
		else
			cast.tex = &g->tex_so;
		cast.texX = (int)(cast.wallx * (float)cast.tex->w);
		if (cast.side == 0 && cast.ray_dx > 0.0f)
			cast.texX = cast.tex->w - cast.texX - 1;
		if (cast.side == 1 && cast.ray_dy < 0.0f)
			cast.texX = cast.tex->w - cast.texX - 1;
		cast.step = (float)cast.tex->h / (float)cast.line_h;
		cast.texPos = (cast.draw_start - (W_HEIGHT / 2 - cast.line_h / 2)) * cast.step;
		cast.y = cast.draw_start;
		while (cast.y <= cast.draw_end)
		{
			cast.texY = (int)cast.texPos;
			cast.c = texel_at(cast.tex, cast.texX, cast.texY);
			if (cast.side == 1)
				cast.c = ((cast.c & 0xFEFEFE) >> 1);
			put_pixel(cast.x, cast.y, cast.c, g);
			cast.texPos += cast.step;
			cast.y++;
		}
		cast.x++;
	}
}
