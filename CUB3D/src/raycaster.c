/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:20:00 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/17 20:19:16 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	init_ray_params(t_game *g, int x, t_cast *cast, float dir0,
		float step_ang)
{
	float	posXfrac;
	float	posYfrac;
	float	deltaDistX;
	float	deltaDistY;

	cast->ray_ang = dir0 + step_ang * (float)x;
	cast->dx = cosf(cast->ray_ang);
	cast->dy = sinf(cast->ray_ang);
	cast->map_x = (int)(g->pl.x / (float)TILE);
	cast->map_y = (int)(g->pl.y / (float)TILE);
	posXfrac = (g->pl.x / (float)TILE) - (int)(g->pl.x / (float)TILE);
	posYfrac = (g->pl.y / (float)TILE) - (int)(g->pl.y / (float)TILE);
	if (cast->dx == 0.0f)
		deltaDistX = 1e30f;
	else
		deltaDistX = ft_fabsf(1.0f / cast->dx);
	if (cast->dy == 0.0f)
		deltaDistY = 1e30f;
	else
		deltaDistY = ft_fabsf(1.0f / cast->dy);
	if (cast->dx < 0.0f)
		cast->step_x = -1;
	else
		cast->step_x = 1;
	if (cast->dy < 0.0f)
		cast->step_y = -1;
	else
		cast->step_y = 1;
	if (cast->dx < 0.0f)
		cast->side_dx = posXfrac * deltaDistX;
	else
		cast->side_dx = (1.0f - posXfrac) * deltaDistX;
	if (cast->dy < 0.0f)
		cast->side_dy = posYfrac * deltaDistY;
	else
		cast->side_dy = (1.0f - posYfrac) * deltaDistY;
	cast->side = -1;
}

static void	perform_dda(t_game *g, t_cast *cast)
{
	float	delta_dist_x;
	float	delta_dist_y;

	if (cast->dx == 0.0f)
		delta_dist_x = 1e30f;
	else
		delta_dist_x = ft_fabsf(1.0f / cast->dx);
	if (cast->dy == 0.0f)
		delta_dist_y = 1e30f;
	else
		delta_dist_y = ft_fabsf(1.0f / cast->dy);
	while (1)
	{
		if (cast->side_dx < cast->side_dy)
		{
			cast->side_dx += delta_dist_x;
			cast->map_x += cast->step_x;
			cast->side = 0;
		}
		else
		{
			cast->side_dy += delta_dist_y;
			cast->map_y += cast->step_y;
			cast->side = 1;
		}
		if (is_wall(&g->sc, cast->map_x, cast->map_y))
			break ;
	}
}

static void	distance_and_height(t_game *g, t_cast *cast, float posX, float posY)
{
	int	line_h;

	if (cast->side == 0)
	{
		if (cast->step_x < 0)
			cast->euclidean_dist = (cast->map_x - posX + 1.0f) / cast->dx;
		else
			cast->euclidean_dist = (cast->map_x - posX) / cast->dx;
	}
	else
	{
		if (cast->step_y < 0)
			cast->euclidean_dist = (cast->map_y - posY + 1.0f) / cast->dy;
		else
			cast->euclidean_dist = (cast->map_y - posY) / cast->dy;
	}
	if (cast->euclidean_dist < 0.6f)
		cast->euclidean_dist = 0.8f;
	if (cast->euclidean_dist < 0.1f)
		cast->euclidean_dist = 0.4f;
	cast->perp = cast->euclidean_dist * cosf(g->pl.dir - cast->ray_ang);
	if (cast->perp < 0.1f)
		cast->perp = 0.1f;
	line_h = (int)(W_HEIGHT / cast->perp);
	cast->top = -line_h / 2 + W_HEIGHT / 2;
	cast->bot = line_h / 2 + W_HEIGHT / 2;
	if (cast->top < 0)
		cast->top = 0;
	if (cast->bot >= W_HEIGHT)
		cast->bot = W_HEIGHT - 1;
}

static t_texture	*texture_and_coords(t_game *g, t_cast *cast, float posX,
		float posY)
{
	t_texture	*tex;

	if (cast->side == 0)
		cast->wallx = posY + cast->euclidean_dist * cast->dy;
	else
		cast->wallx = posX + cast->euclidean_dist * cast->dx;
	cast->wallx -= (int)cast->wallx;
	if (cast->wallx < 0.0f)
		cast->wallx += 1.0f;
	if (cast->wallx >= 1.0f)
		cast->wallx -= 1.0f;
	if (cast->side == 0 && cast->step_x < 0)
		tex = &g->tex_we;
	else if (cast->side == 0)
		tex = &g->tex_ea;
	else if (cast->side == 1 && cast->step_y < 0)
		tex = &g->tex_no;
	else
		tex = &g->tex_so;
	cast->tex_x = (int)(cast->wallx * (float)tex->w);
	if (cast->side == 0 && cast->dx > 0.0f)
		cast->tex_x = tex->w - cast->tex_x - 1;
	else if (cast->side == 1 && cast->dy < 0.0f)
		cast->tex_x = tex->w - cast->tex_x - 1;
	return (tex);
}

static void	draw_wall_strip(t_game *g, int x, t_cast *cast, t_texture *tex)
{
	float			step;
	float			texPos;
	int				y;
	int				texY;
	unsigned int	c;
	int				line_height;

	line_height = cast->bot - cast->top + 1;
	if (line_height < 1)
		line_height = 1;
	step = (float)tex->h / (float)line_height;
	texPos = (cast->top - (W_HEIGHT / 2 - line_height / 2)) * step;
	y = cast->top;
	while (y <= cast->bot)
	{
		texY = (int)texPos;
		if (texY < 0)
			texY = 0;
		if (texY >= tex->h)
			texY = tex->h - 1;
		c = texel_at(tex, cast->tex_x, texY);
		if (cast->side == 1)
			c = ((c & 0xFEFEFE) >> 1);
		put_pixel(x, y, c, g);
		texPos += step;
		y++;
	}
}

void	cast_and_draw_all(t_game *g)
{
	float		posY;
	int			x;
	t_cast		cast;
	t_texture	*tex;
	float		fov;
	float		dir0;
	float		step_ang;
	float		posX;

	fov = 50.0f * PI_VAL / 180.0f;
	dir0 = g->pl.dir - fov / 2.0f;
	step_ang = fov / (float)W_WIDTH;
	posX = g->pl.x / (float)TILE;
	posY = g->pl.y / (float)TILE;
	x = 0;
	while (x < W_WIDTH)
	{
		init_ray_params(g, x, &cast, dir0, step_ang);
		perform_dda(g, &cast);
		distance_and_height(g, &cast, posX, posY);
		tex = texture_and_coords(g, &cast, posX, posY);
		draw_wall_strip(g, x, &cast, tex);
		x++;
	}
}
