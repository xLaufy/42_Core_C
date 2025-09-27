/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:34:04 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/09/27 19:44:03 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	push_line(t_scene *sc, char *ln)
{
	char	**n;
	int		i;
	int		w;
	char	*clean;

	clean = dup_no_cr(ln);
	if (!clean)
		exit(1);
	n = (char **)malloc(sizeof(char *) * (sc->h + 2));
	if (!n)
		exit(1);
	i = 0;
	while (i < sc->h)
	{
		n[i] = sc->map[i];
		i++;
	}
	n[sc->h] = clean;
	n[sc->h + 1] = NULL;
	free(sc->map);
	sc->map = n;
	sc->h = sc->h + 1;
	w = (int)ft_strlen(clean);
	if (w > sc->w)
		sc->w = w;
}

static void	rect_pad(t_scene *sc)
{
	int		y;
	int		len;
	char	*row;

	y = 0;
	while (y < sc->h)
	{
		len = (int)ft_strlen(sc->map[y]);
		if (len < sc->w)
		{
			row = (char *)malloc(sc->w + 1);
			if (!row)
				exit(1);
			ft_memset(row, ' ', sc->w);
			if (len > 0)
				ft_memcpy(row, sc->map[y], len);
			row[sc->w] = '\0';
			free(sc->map[y]);
			sc->map[y] = row;
		}
		y++;
	}
}

static void	check_spawn_and_closed(t_scene *sc)
{
	int	y;
	int	cnt;

	cnt = 0;
	y = 0;
	while (y < sc->h)
	{
		check_spawn_and_closed_row(sc, y, &cnt);
		y++;
	}
	if (cnt != 1)
		free_scene_exit(sc);
}

void	parse_cub(const char *path, t_scene *sc)
{
	char	**ls;
	int		i;
	char	*trim;

	ft_bzero(sc, sizeof(*sc));
	ls = load_lines(path);
	if (!ls)
		exit(1);
	i = skip_ids_and_empty(ls, sc);
	while (ls[i])
	{
		push_line(sc, ls[i]);
		i++;
	}
	ft_free_split(ls);
	if (!sc->no || !sc->so || !sc->we || !sc->ea)
		exit(1);
	rect_pad(sc);
	check_spawn_and_closed(sc);
}

void	free_scene(t_scene *sc)
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
}
