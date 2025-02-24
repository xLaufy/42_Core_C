/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/09/21 10:10:41 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/09/21 10:10:41 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

void	instructions(void)
{
	ft_printf("\nPress to move: W, A, S, D or Left, Right, Up, Down arrows\n");
	ft_printf("\nUse mouse scroll to zoom\n");
	ft_printf("\nPress SPACEBAR for color switch\n");
	ft_printf("\nTo rotate Julia:	Mouse left or right click\n");
	ft_printf("\nTo close the window:\nPress key: ESC\nClick X on window\n");
	ft_printf("\nTry for Julia: <./fractol julia -0.4 +0.6 or\n");
	ft_printf("./fractol julia -0.835 -0.2321\n");
}

void	malloc_error(void)
{
	perror("MALLOC ERROR:");
	exit(EXIT_FAILURE);
}
