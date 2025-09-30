/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 21:14:23 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/30 21:14:36 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	foreign_symbol(t_scene *sc, int x, int y)
{
	if (sc->map[y][x] != '0' && sc->map[y][x] != '1' && sc->map[y][x] != ' '
		&& sc->map[y][x] != 'N' && sc->map[y][x] != 'S' && sc->map[y][x] != 'E'
		&& sc->map[y][x] != 'W')
		free_scene_exit(sc);
}
