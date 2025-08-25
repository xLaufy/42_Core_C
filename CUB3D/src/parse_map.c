/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:34:04 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/08/25 17:34:04 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int					set_id(t_scene *sc, char *line);
char				**load_lines(const char *path);

static const char	*skip_ws(const char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}

static int	is_id_line(const char *raw)
{
	const char	*p;

	p = skip_ws(raw);
	if (p[0] == 'N' && p[1] == 'O' && (p[2] == ' ' || p[2] == '\t'))
		return (1);
	if (p[0] == 'S' && p[1] == 'O' && (p[2] == ' ' || p[2] == '\t'))
		return (1);
	if (p[0] == 'W' && p[1] == 'E' && (p[2] == ' ' || p[2] == '\t'))
		return (1);
	if (p[0] == 'E' && p[1] == 'A' && (p[2] == ' ' || p[2] == '\t'))
		return (1);
	if (p[0] == 'F' && (p[1] == ' ' || p[1] == '\t'))
		return (1);
	if (p[0] == 'C' && (p[1] == ' ' || p[1] == '\t'))
		return (1);
	return (0);
}

static char	*dup_no_cr(const char *ln)
{
	size_t	len;
	char	*out;

	len = ft_strlen(ln);
	if (len && ln[len - 1] == '\r')
		len--;
	out = (char *)malloc(len + 1);
	if (!out)
		return (NULL);
	if (len)
		ft_memcpy(out, ln, len);
	out[len] = '\0';
	return (out);
}

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
	int	x;
	int	cnt;

	cnt = 0;
	y = 0;
	while (y < sc->h)
	{
		x = 0;
		while (x < sc->w)
		{
			if (sc->map[y][x] == 'N' || sc->map[y][x] == 'S'
				|| sc->map[y][x] == 'E' || sc->map[y][x] == 'W')
			{
				sc->spawn = sc->map[y][x];
				cnt++;
			}
			if (sc->map[y][x] == '0' || sc->map[y][x] == 'N'
				|| sc->map[y][x] == 'S' || sc->map[y][x] == 'E'
				|| sc->map[y][x] == 'W')
			{
				if (y == 0 || x == 0 || y == sc->h - 1 || x == sc->w - 1)
					exit(1);
				if (sc->map[y - 1][x] == ' ' || sc->map[y + 1][x] == ' '
					|| sc->map[y][x - 1] == ' ' || sc->map[y][x + 1] == ' ')
					exit(1);
			}
			x++;
		}
		y++;
	}
	if (cnt != 1)
		exit(1);
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
	i = 0;
	while (ls[i])
	{
		trim = ft_strtrim(ls[i], " \t\r\n");
		if (!trim)
			exit(1);
		if (trim[0] == '\0')
		{
			free(trim);
			i++;
			continue ;
		}
		if (is_id_line(ls[i]))
		{
			if (!set_id(sc, trim))
			{
				free(trim);
				exit(1);
			}
			free(trim);
			i++;
			continue ;
		}
		free(trim);
		break ;
	}
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
