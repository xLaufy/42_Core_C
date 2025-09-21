/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:33:53 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/09/21 18:39:00 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	handle_input(t_game *g)
{
	t_input	input;

	ft_bzero(&input, 0);
	input.mv = 3.0f;
	input.rt = 0.06f;
	if (g->pl.k_l)
		g->pl.dir -= input.rt;
	if (g->pl.k_r)
		g->pl.dir += input.rt;
	input.cs = cosf(g->pl.dir);
	input.sn = sinf(g->pl.dir);
	key_press_collision(g, &input);
}

int	draw_loop(t_game *g)
{
	int		y;
	int		x;
	size_t	sz;

	sz = (size_t)g->line_len * (size_t)W_HEIGHT;
	ft_bzero(g->data, sz);
	handle_input(g);
	y = 0;
	while (y < W_HEIGHT)
	{
		x = 0;
		while (x < W_WIDTH)
		{
			if (y < W_HEIGHT / 2)
				put_pixel(x, y, g->sc.c_rgb, g);
			else
				put_pixel(x, y, g->sc.f_rgb, g);
			x++;
		}
		y++;
	}
	cast_and_draw_all(g);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	return (0);
}

int	main(int ac, char **av)
{
	t_game	g;

	if (ac != 2)
		return (1);
	ft_bzero(&g, sizeof(g));
	init_game(&g, av[1]);
	mlx_hook(g.win, 17, 0, close_game, &g);
	mlx_hook(g.win, 2, 1L << 0, key_press, &g);
	mlx_hook(g.win, 3, 1L << 1, key_release, &g);
	mlx_loop_hook(g.mlx, draw_loop, &g);
	mlx_loop(g.mlx);
	return (0);
}
