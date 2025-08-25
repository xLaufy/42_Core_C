/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:34:14 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/08/25 17:34:14 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	is_wall(t_scene *sc, int mx, int my)
{
	if (mx < 0 || my < 0 || mx >= sc->w || my >= sc->h)
		return (1);
	return (sc->map[my][mx] == '1');
}

static unsigned int	texel_at(t_texture *t, int tx, int ty)
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

void	cast_and_draw_all(t_game *g)
{
	const float		fov;
	const float		dir0;
	const float		step_ang;
	const float		posX;
	const float		posY;
	int				x;
	float			ray_ang;
	float			ray_dx;
	float			ray_dy;
	int				mapX;
	int				mapY;
		float sideDistX;
		float sideDistY;
	float			deltaDistX;
	float			deltaDistY;
	int				stepX;
	int				stepY;
	float			posXfrac;
	float			posYfrac;
	int				side;
		float perp;
	int				line_h;
	int				draw_start;
	int				draw_end;
		float wallx;
		t_texture *tex;
	int				texX;
	float			step;
	float			texPos;
	int				y;
	int				texY;
	unsigned int	c;

	fov = 60.0f * PI_VAL / 180.0f;
	dir0 = g->pl.dir - fov / 2.0f;
	step_ang = fov / (float)W_WIDTH;
	posX = g->pl.x / (float)TILE;
	posY = g->pl.y / (float)TILE;
	x = 0;
	while (x < W_WIDTH)
	{
		ray_ang = dir0 + step_ang * (float)x;
		ray_dx = cosf(ray_ang);
		ray_dy = sinf(ray_ang);
		mapX = (int)posX;
		mapY = (int)posY;
		deltaDistX = (ray_dx == 0.0f) ? 1e30f : fabsf(1.0f / ray_dx);
		deltaDistY = (ray_dy == 0.0f) ? 1e30f : fabsf(1.0f / ray_dy);
		stepX = (ray_dx < 0.0f) ? -1 : 1;
		stepY = (ray_dy < 0.0f) ? -1 : 1;
		posXfrac = posX - floorf(posX);
		posYfrac = posY - floorf(posY);
		if (ray_dx < 0.0f)
			sideDistX = posXfrac * deltaDistX;
		else
			sideDistX = (1.0f - posXfrac) * deltaDistX;
		if (ray_dy < 0.0f)
			sideDistY = posYfrac * deltaDistY;
		else
			sideDistY = (1.0f - posYfrac) * deltaDistY;
		side = -1;
		while (1)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (is_wall(&g->sc, mapX, mapY))
				break ;
		}
		if (side == 0)
			perp = (mapX - posX + (stepX < 0 ? 1.0f : 0.0f)) / ray_dx;
		else
			perp = (mapY - posY + (stepY < 0 ? 1.0f : 0.0f)) / ray_dy;
		line_h = (int)(W_HEIGHT / perp);
		draw_start = -line_h / 2 + W_HEIGHT / 2;
		draw_end = line_h / 2 + W_HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;
		if (draw_end >= W_HEIGHT)
			draw_end = W_HEIGHT - 1;
		if (side == 0)
			wallx = posY + perp * ray_dy;
		else
			wallx = posX + perp * ray_dx;
		wallx -= floorf(wallx);
		if (side == 0 && stepX < 0)
			tex = &g->tex_we;
		else if (side == 0)
			tex = &g->tex_ea;
		else if (side == 1 && stepY < 0)
			tex = &g->tex_no;
		else
			tex = &g->tex_so;
		texX = (int)(wallx * (float)tex->w);
		if (side == 0 && ray_dx > 0.0f)
			texX = tex->w - texX - 1;
		if (side == 1 && ray_dy < 0.0f)
			texX = tex->w - texX - 1;
		step = (float)tex->h / (float)line_h;
		texPos = (draw_start - (W_HEIGHT / 2 - line_h / 2)) * step;
		y = draw_start;
		while (y <= draw_end)
		{
			texY = (int)texPos;
			c = texel_at(tex, texX, texY);
			if (side == 1)
				c = ((c & 0xFEFEFE) >> 1);
			put_pixel(x, y, c, g);
			texPos += step;
			y++;
		}
		x++;
	}
}
