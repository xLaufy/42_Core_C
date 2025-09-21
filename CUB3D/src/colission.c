/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colission.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 18:50:27 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/21 19:11:15 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

t_colon	init_colon(float new_x, float new_y)
{
	t_colon	colon;

	ft_bzero(&colon, 0);
	colon.margin = 0.1f;
	colon.map_x = (int)(new_x / TILE);
	colon.map_y = (int)(new_y / TILE);
	colon.frac_x = new_x / TILE - colon.map_x;
	colon.frac_y = new_y / TILE - colon.map_y;
	return (colon);
}

int	check_collision(t_scene *sc, float new_x, float new_y)
{
	t_colon	colon;

	colon = init_colon(new_x, new_y);
	if (is_wall(sc, colon.map_x, colon.map_y))
		return (1);
	if (colon.frac_x < colon.margin && is_wall(sc, colon.map_x - 1,
			colon.map_y))
		return (1);
	if (colon.frac_x > 1.0f - colon.margin && is_wall(sc, colon.map_x + 1,
			colon.map_y))
		return (1);
	if (colon.frac_y < colon.margin && is_wall(sc, colon.map_x, colon.map_y
			- 1))
		return (1);
	if (colon.frac_y > 1.0f - colon.margin && is_wall(sc, colon.map_x,
			colon.map_y + 1))
		return (1);
	return (0);
}
