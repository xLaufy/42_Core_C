/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 20:32:51 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/21 20:54:43 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	check_spawn_and_closed_row(t_scene *sc, int y, int *cnt)
{
	int	x;

	x = 0;
	while (x < sc->w)
	{
		if (sc->map[y][x] == 'N' || sc->map[y][x] == 'S' || sc->map[y][x] == 'E'
			|| sc->map[y][x] == 'W')
		{
			sc->spawn = sc->map[y][x];
			(*cnt)++;
		}
		if (sc->map[y][x] == '0' || sc->map[y][x] == 'N' || sc->map[y][x] == 'S'
			|| sc->map[y][x] == 'E' || sc->map[y][x] == 'W')
		{
			if (y == 0 || x == 0 || y == sc->h - 1 || x == sc->w - 1)
				exit(1);
			if (sc->map[y - 1][x] == ' ' || sc->map[y + 1][x] == ' '
				|| sc->map[y][x - 1] == ' ' || sc->map[y][x + 1] == ' ')
				exit(1);
		}
		x++;
	}
}

int	set_id_keyval(t_scene *sc, const char *key, const char *val)
{
	if (!ft_strncmp(key, "NO", 3))
		sc->no = ft_strdup(val);
	else if (!ft_strncmp(key, "SO", 3))
		sc->so = ft_strdup(val);
	else if (!ft_strncmp(key, "WE", 3))
		sc->we = ft_strdup(val);
	else if (!ft_strncmp(key, "EA", 3))
		sc->ea = ft_strdup(val);
	else if (!ft_strncmp(key, "F", 2))
		return (parse_rgb_num((char *)val, &sc->f_rgb));
	else if (!ft_strncmp(key, "C", 2))
		return (parse_rgb_num((char *)val, &sc->c_rgb));
	else
		return (0);
	return (1);
}
