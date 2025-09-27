/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 19:13:03 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/27 18:59:12 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

const char	*skip_ws(char const *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}

int	is_id_line(const char *raw)
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

char	*dup_no_cr(const char *ln)
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

static int	handle_id_line(char **ls, t_scene *sc, char *trim, int i)
{
	if (!set_id(sc, trim))
	{
		free(trim);
		exit(1);
	}
	free(trim);
	return (i + 1);
}

int	skip_ids_and_empty(char **ls, t_scene *sc)
{
	int		i;
	char	*trim;

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
			i = handle_id_line(ls, sc, trim, i);
			continue ;
		}
		free(trim);
		break ;
	}
	return (i);
}
