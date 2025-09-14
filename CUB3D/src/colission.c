/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colission.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 18:50:27 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/14 20:53:33 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	check_collision(t_scene *sc, float new_x, float new_y)
{
	int		mapX;
	int		mapY;
	float	radius;

	radius = TILE * 0.2f;
	mapX = (int)(new_x / TILE);
	mapY = (int)(new_y / TILE);
	if (is_wall(sc, mapX, mapY))
		return (1);
	if (is_wall(sc, (int)((new_x - radius) / TILE), (int)((new_y - radius)
				/ TILE)))
		return (1);
	if (is_wall(sc, (int)((new_x + radius) / TILE), (int)((new_y - radius)
				/ TILE)))
		return (1);
	if (is_wall(sc, (int)((new_x - radius) / TILE), (int)((new_y + radius)
				/ TILE)))
		return (1);
	if (is_wall(sc, (int)((new_x + radius) / TILE), (int)((new_y + radius)
				/ TILE)))
		return (1);
	return (0);
}
