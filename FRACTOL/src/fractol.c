/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/09/21 10:10:59 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/09/21 10:10:59 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fractol.h"

int	main(int argc, char **argv)
{
	t_fractol	fract;

	if ((argc == 2 && !ft_strncmp(argv[1], "mandelbrot", ft_strlen("mandelbrot")
				+ 1)) || (argc == 4 && !ft_strncmp(argv[1], "julia",
				ft_strlen("julia") + 1)))
	{
		instructions();
		argv[1][0] = ft_toupper(argv[1][0]);
		fract.title = argv[1];
		if (!ft_strncmp(fract.title, "Julia", 5))
		{
			fract.julia_r = atod(argv[2]);
			fract.julia_i = atod(argv[3]);
		}
		fractol_init(&fract);
		render_fractal(&fract);
		mlx_loop(fract.mlx_connect);
		return (0);
	}
	ft_printf("Invalid input\n");
	ft_printf("\nTry for Mandelbrot: ./fractol mandelbrot\n");
	ft_printf("\nTry for Julia: ./fractol julia -0.4 +0.6 or\n");
	ft_printf("./fractol julia -0.835 -0.2321\n");
	exit(EXIT_FAILURE);
}
