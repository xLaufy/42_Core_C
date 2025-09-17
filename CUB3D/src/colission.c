/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colission.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 18:50:27 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/17 20:03:30 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int check_collision(t_scene *sc, float new_x, float new_y)
{
    float margin = 0.2f;
    int mapX = (int)(new_x / TILE);
    int mapY = (int)(new_y / TILE);
    float fracX = new_x / TILE - mapX;
    float fracY = new_y / TILE - mapY;

    if (is_wall(sc, mapX, mapY))
        return (1);
    if (fracX < margin && is_wall(sc, mapX - 1, mapY))
        return (1);
    if (fracX > 1.0f - margin && is_wall(sc, mapX + 1, mapY))
        return (1);
    if (fracY < margin && is_wall(sc, mapX, mapY - 1))
        return (1);
    if (fracY > 1.0f - margin && is_wall(sc, mapX, mapY + 1))
        return (1);
    return (0);
}
