/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_press2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 17:02:36 by rkobelie          #+#    #+#             */
/*   Updated: 2025/09/21 17:03:21 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	key_press(int k, t_game *g)
{
	if (k == KEY_ESC)
		return (close_game(g));
	if (k == KEY_W)
		g->pl.k_w = true;
	if (k == KEY_S)
		g->pl.k_s = true;
	if (k == KEY_A)
		g->pl.k_a = true;
	if (k == KEY_D)
		g->pl.k_d = true;
	if (k == KEY_L)
		g->pl.k_l = true;
	if (k == KEY_R)
		g->pl.k_r = true;
	return (0);
}

int	key_release(int k, t_game *g)
{
	if (k == KEY_W)
		g->pl.k_w = false;
	if (k == KEY_S)
		g->pl.k_s = false;
	if (k == KEY_A)
		g->pl.k_a = false;
	if (k == KEY_D)
		g->pl.k_d = false;
	if (k == KEY_L)
		g->pl.k_l = false;
	if (k == KEY_R)
		g->pl.k_r = false;
	return (0);
}
