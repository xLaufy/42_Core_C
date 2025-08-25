/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/08/25 17:33:56 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/08/25 17:33:56 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	draw_square(int x, int y, int size, int color, t_game *game)
{
	for (int i = 0; i < size; i++)
		put_pixel(x + i, y, color, game);
	for (int i = 0; i < size; i++)
		put_pixel(x, y + i, color, game);
	for (int i = 0; i < size; i++)
		put_pixel(x + size, y + i, color, game);
	for (int i = 0; i < size; i++)
		put_pixel(x + i, y + size, color, game);
}

void	draw_map(t_game *game)
{
	char	**map;
	int		color;

	color = 0x0000FF;
	map = game->map;
	for (int y = 0; map[y]; y++)
		for (int x = 0; map[y][x]; x++)
			if (map[y][x] == '1')
				draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
}

draw_square((int)game->player.x, (int)game->player.y, 10, 0x00FF00, game);
draw_map(game);
