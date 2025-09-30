/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 20:34:51 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/30 20:34:54 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	safe_set_texture(char **dst, const char *val)
{
	if (*dst)
		free(*dst);
	*dst = ft_strdup(val);
}

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
				free_scene_exit(sc);
			if (sc->map[y - 1][x] == ' ' || sc->map[y + 1][x] == ' '
				|| sc->map[y][x - 1] == ' ' || sc->map[y][x + 1] == ' ')
				free_scene_exit(sc);
		}
		x++;
	}
}

int	set_id_keyval(t_scene *sc, const char *key, const char *val)
{
	if (!ft_strncmp(key, "NO", 3))
		safe_set_texture(&sc->no, val);
	else if (!ft_strncmp(key, "SO", 3))
		safe_set_texture(&sc->so, val);
	else if (!ft_strncmp(key, "WE", 3))
		safe_set_texture(&sc->we, val);
	else if (!ft_strncmp(key, "EA", 3))
		safe_set_texture(&sc->ea, val);
	else if (!ft_strncmp(key, "F", 2))
		return (parse_rgb_num((char *)val, &sc->f_rgb));
	else if (!ft_strncmp(key, "C", 2))
		return (parse_rgb_num((char *)val, &sc->c_rgb));
	else
		return (0);
	return (1);
}

void	free_scene_exit(t_scene *sc)
{
	int	y;

	free(sc->no);
	free(sc->so);
	free(sc->we);
	free(sc->ea);
	y = 0;
	while (y < sc->h)
	{
		free(sc->map[y]);
		y++;
	}
	free(sc->map);
	printf("Error:: Check your .cub file\n");
	exit(1);
}
