/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:33:49 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/08/25 17:33:50 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	put_pixel(int x, int y, int color, t_game *g)
{
	int	i;

	if (x < 0 || y < 0)
		return ;
	if (x >= W_WIDTH || y >= W_HEIGHT)
		return ;
	i = y * g->line_len + x * (g->bpp / 8);
	g->data[i + 0] = color & 0xFF;
	g->data[i + 1] = (color >> 8) & 0xFF;
	g->data[i + 2] = (color >> 16) & 0xFF;
}

static void	set_player_from_spawn(t_game *g)
{
	int		y;
	int		x;
	char	c;

	y = 0;
	while (y < g->sc.h)
	{
		x = 0;
		while (x < g->sc.w)
		{
			c = g->sc.map[y][x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				g->pl.x = x * TILE + TILE / 2.0f;
				g->pl.y = y * TILE + TILE / 2.0f;
				if (c == 'N')
					g->pl.dir = -PI_VAL / 2.0f;
				if (c == 'S')
					g->pl.dir = PI_VAL / 2.0f;
				if (c == 'E')
					g->pl.dir = 0.0f;
				if (c == 'W')
					g->pl.dir = PI_VAL;
				g->sc.map[y][x] = '0';
			}
			x++;
		}
		y++;
	}
}

void	init_game(t_game *g, const char *cub_path)
{
	parse_cub(cub_path, &g->sc);
	g->mlx = mlx_init();
	if (!g->mlx)
		exit(1);
	g->win = mlx_new_window(g->mlx, W_WIDTH, W_HEIGHT, "cub3D");
	if (!g->win)
		exit(1);
	g->img = mlx_new_image(g->mlx, W_WIDTH, W_HEIGHT);
	if (!g->img)
		exit(1);
	g->data = mlx_get_data_addr(g->img, &g->bpp, &g->line_len, &g->endian);
	load_textures(g);
	set_player_from_spawn(g);
}

int	close_game(t_game *g)
{
	destroy_textures(g);
	if (g->img)
		mlx_destroy_image(g->mlx, g->img);
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	free_scene(&g->sc);
	exit(0);
	return (0);
}
