/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 18:20:00 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/13 19:06:34 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static float	ft_fabsf(float x)
{
	if (x < 0.0f)
		return (-x);
	return (x);
}

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

static void	init_ray_params(t_game *g, int x, t_cast *cast, float dir0,
		float step_ang)
{
	float	ray_ang;
	float	posXfrac;
	float	posYfrac;
	float	deltaDistX;
	float	deltaDistY;

	ray_ang = dir0 + step_ang * (float)x;
	cast->dx = cosf(ray_ang);                   // kierunex x
	cast->dy = sinf(ray_ang);                   // kierunek y
	cast->map_x = (int)(g->pl.x / (float)TILE); // nastepna klatka
	cast->map_y = (int)(g->pl.y / (float)TILE);
	posXfrac = (g->pl.x / (float)TILE) - (int)(g->pl.x / (float)TILE);
	posYfrac = (g->pl.y / (float)TILE) - (int)(g->pl.y / (float)TILE);
	if (cast->dx == 0.0f)
		deltaDistX = 1e30f; // unikam dzielenie na 0
	else
		deltaDistX = ft_fabsf(1.0f / cast->dx);
	if (cast->dy == 0.0f)
		deltaDistY = 1e30f;
	else
		deltaDistY = ft_fabsf(1.0f / cast->dy);
	// kierunek przesuniencza
	if (cast->dx < 0.0f)
		cast->step_x = -1;
	else
		cast->step_x = 1;
	if (cast->dy < 0.0f)
		cast->step_y = -1;
	else
		cast->step_y = 1;
	// odleglosc do pirwszej linii
	if (cast->dx < 0.0f)
		cast->side_dx = posXfrac * deltaDistX;
	else
		cast->side_dx = (1.0f - posXfrac) * deltaDistX;
	if (cast->dy < 0.0f)
		cast->side_dy = posYfrac * deltaDistY;
	else
		cast->side_dy = (1.0f - posYfrac) * deltaDistY;
	cast->side = -1; // dokad nie znadziemy sciane
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
			cast->side = 1; // horyzontalna sciana
		}
		if (is_wall(&g->sc, cast->map_x, cast->map_y))
			break ;
	}
}

// rozliczenia wysok. ta odl. od sciany
static void	distance_and_height(t_game *g, t_cast *cast, float posX,
		float posY)
{
	if (cast->side == 0)
	{
		if (cast->step_x < 0)
			cast->perp = (cast->map_x - posX + 1.0f) / cast->dx;
		else
			cast->perp = (cast->map_x - posX) / cast->dx;
	}
	else
	{
		if (cast->step_y < 0)
			cast->perp = (cast->map_y - posY + 1.0f) / cast->dy;
		else
			cast->perp = (cast->map_y - posY) / cast->dy;
	}
	// dzielenie na 0
	if (cast->perp < 0.01f)
		cast->perp = 0.01f;
	int line_h = (int)(W_HEIGHT / cast->perp); // wysokosc
	cast->top = -line_h / 2 + W_HEIGHT / 2;    // poczatek
	cast->bot = line_h / 2 + W_HEIGHT / 2;     // koniec linii
	if (cast->top < 0)
		cast->top = 0;
	if (cast->bot >= W_HEIGHT)
		cast->bot = W_HEIGHT - 1;
}

// textury ta coords
static t_texture	*texture_and_coords(t_game *g, t_cast *cast,
		float posX, float posY)
{
	t_texture	*tex;

	// coord wtrap w sciane
	if (cast->side == 0)
		cast->wallx = posY + cast->perp * cast->dy;
	else
		cast->wallx = posX + cast->perp * cast->dx;
	cast->wallx -= floorf(cast->wallx);
	// wybor textury
	if (cast->side == 0 && cast->step_x < 0)
		tex = &g->tex_we; // sciany
	else if (cast->side == 0)
		tex = &g->tex_ea;
	else if (cast->side == 1 && cast->step_y < 0)
		tex = &g->tex_no;
	else
		tex = &g->tex_so;
	// x textury
	cast->tex_x = (int)(cast->wallx * (float)tex->w);
	if (cast->side == 0 && cast->dx > 0.0f)
		cast->tex_x = tex->w - cast->tex_x - 1;
	else if (cast->side == 1 && cast->dy < 0.0f)
		cast->tex_x = tex->w - cast->tex_x - 1;
	return (tex);
}

// rys linii
static void	draw_wall_strip(t_game *g, int x, t_cast *cast, t_texture *tex)
{
	float	step;
	float	texPos;
	int		y;
	int		texY;

	step = (float)tex->h / (float)(cast->bot - cast->top + 1);
	texPos = (cast->top - (W_HEIGHT / 2 - (cast->bot - cast->top + 1) / 2))
		* step;
	y = cast->top;
	while (y <= cast->bot)
	{
		texY = (int)texPos;
		unsigned int c = texel_at(tex, cast->tex_x, texY);
		if (cast->side == 1)
			c = ((c & 0xFEFEFE) >> 1); // dla cieni
		put_pixel(x, y, c, g);         // draw pixel
		texPos += step;
		y++;
	}
}

void	cast_and_draw_all(t_game *g)
{
	float	posY;
	int		x;

	float fov = 30.0f * PI_VAL / 180.0f;   // kat wzroku
	float dir0 = g->pl.dir - fov / 2.0f;   // kat sprawdania
	float step_ang = fov / (float)W_WIDTH; // kat stepu
	float posX = g->pl.x / (float)TILE;    // pozycja gracza w tailach
	posY = g->pl.y / (float)TILE;
	x = 0;
	while (x < W_WIDTH)
	{
		t_cast cast;
		init_ray_params(g, x, &cast, dir0, step_ang);
		perform_dda(g, &cast);
		calculate_distance_and_height(g, &cast, posX, posY);
		t_texture *tex = select_texture_and_coords(g, &cast, posX, posY);
		draw_wall_strip(g, x, &cast, tex);
		x++;
	}
}
