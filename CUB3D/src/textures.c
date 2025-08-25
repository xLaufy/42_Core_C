/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:34:18 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/08/25 17:34:18 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	load_one(t_game *g, t_texture *t, const char *p)
{
	t->img = mlx_xpm_file_to_image(g->mlx, (char *)p, &t->w, &t->h);
	if (!t->img)
		exit(1);
	t->data = mlx_get_data_addr(t->img, &t->bpp, &t->line_len, &t->endian);
}

void	load_textures(t_game *g)
{
	load_one(g, &g->tex_no, g->sc.no);
	load_one(g, &g->tex_so, g->sc.so);
	load_one(g, &g->tex_we, g->sc.we);
	load_one(g, &g->tex_ea, g->sc.ea);
}

void	destroy_textures(t_game *g)
{
	if (g->tex_no.img)
		mlx_destroy_image(g->mlx, g->tex_no.img);
	if (g->tex_so.img)
		mlx_destroy_image(g->mlx, g->tex_so.img);
	if (g->tex_we.img)
		mlx_destroy_image(g->mlx, g->tex_we.img);
	if (g->tex_ea.img)
		mlx_destroy_image(g->mlx, g->tex_ea.img);
}
