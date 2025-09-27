/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_id.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 20:09:35 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/27 20:39:02 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static char	*read_all(const char *p)
{
	int		fd;
	ssize_t	rb;
	char	buf[1024];
	char	*all;
	char	*tmp;

	all = ft_strdup("");
	fd = open(p, O_RDONLY);
	if (fd < 0)
	{
		free(all);
		return (NULL);
	}
	rb = read(fd, buf, 1023);
	while (rb > 0)
	{
		buf[rb] = '\0';
		tmp = all;
		all = ft_strjoin(all, buf);
		free(tmp);
		rb = read(fd, buf, 1023);
	}
	close(fd);
	return (all);
}

char	**load_lines(const char *path)
{
	char	*all;
	char	**ls;

	all = read_all(path);
	if (!all)
		free(all);
	return (NULL);
	ls = ft_split(all, '\n');
	free(all);
	return (ls);
}

int	parse_rgb_num(char *s, int *out_rgb)
{
	char	**v;
	int		r;
	int		g;
	int		b;

	v = ft_split(s, ',');
	if (!v || !v[0] || !v[1] || !v[2] || v[3])
	{
		ft_free_split(v);
		return (0);
	}
	r = ft_atoi(v[0]);
	g = ft_atoi(v[1]);
	b = ft_atoi(v[2]);
	ft_free_split(v);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (0);
	*out_rgb = ((r & 255) << 16) | ((g & 255) << 8) | (b & 255);
	return (1);
}

int	set_id(t_scene *sc, char *line)
{
	t_parse_id	parse;
	int			i;

	parse.s = ft_strtrim(line, " \t\r\n");
	if (!parse.s)
		return (0);
	i = 0;
	while (parse.s[i] && parse.s[i] != ' ' && parse.s[i] != '\t')
		i++;
	parse.key = ft_substr(parse.s, 0, i);
	while (parse.s[i] == ' ' || parse.s[i] == '\t')
		i++;
	parse.val = ft_strdup(parse.s + i);
	free(parse.s);
	if (!parse.key || !parse.val)
	{
		free(parse.key);
		free(parse.val);
		return (0);
	}
	parse.ok = set_id_keyval(sc, parse.key, parse.val);
	free(parse.key);
	free(parse.val);
	return (parse.ok);
}
