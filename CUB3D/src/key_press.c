/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_press.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 17:00:48 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/21 19:21:48 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	w_key(t_game *g, t_input *input)
{
	input->new_x = g->pl.x + input->cs * input->mv;
	input->new_y = g->pl.y + input->sn * input->mv;
	if (!check_collision(&g->sc, input->new_x, input->new_y))
	{
		g->pl.x = input->new_x;
		g->pl.y = input->new_y;
	}
}

void	s_key(t_game *g, t_input *input)
{
	input->new_x = g->pl.x - input->cs * input->mv;
	input->new_y = g->pl.y - input->sn * input->mv;
	if (!check_collision(&g->sc, input->new_x, input->new_y))
	{
		g->pl.x = input->new_x;
		g->pl.y = input->new_y;
	}
}

void	a_key(t_game *g, t_input *input)
{
	input->new_x = g->pl.x + input->sn * input->mv;
	input->new_y = g->pl.y - input->cs * input->mv;
	if (!check_collision(&g->sc, input->new_x, input->new_y))
	{
		g->pl.x = input->new_x;
		g->pl.y = input->new_y;
	}
}

void	d_key(t_game *g, t_input *input)
{
	input->new_x = g->pl.x - input->sn * input->mv;
	input->new_y = g->pl.y + input->cs * input->mv;
	if (!check_collision(&g->sc, input->new_x, input->new_y))
	{
		g->pl.x = input->new_x;
		g->pl.y = input->new_y;
	}
}

void	key_press_collision(t_game *g, t_input *input)
{
	if (g->pl.k_w)
		w_key(g, input);
	if (g->pl.k_s)
		s_key(g, input);
	if (g->pl.k_a)
		a_key(g, input);
	if (g->pl.k_d)
		d_key(g, input);
}
