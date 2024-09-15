#include "../include/fractol.h"

void	instructions(void)
{
	ft_printf("\nTo move the fractal:	Press keys: W, A, S, D or Left, Right, Up, Down arrows\n");
	ft_printf("To zoom in/out use mouse scroll\n");
	ft_printf("Press SPACEBAR for color switch\n");
	ft_printf("To rotate Julia:	Mouse left or right click\n");
	ft_printf("To quit:\nPress key: ESC\nClick X on window\n");
	ft_printf("\nTry: <./fractol julia -0.4 +0.6 or\n<./fractol julia -0.835 -0.2321\n");
}

void	malloc_error(void)
{
	perror("MALLOC ERROR: ");
	exit(EXIT_FAILURE);
}