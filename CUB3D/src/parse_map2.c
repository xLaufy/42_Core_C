/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 19:13:03 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/21 19:16:53 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

char	*skip_ws(const char *s)
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
