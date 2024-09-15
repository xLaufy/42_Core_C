#include "../include/fractol.h"

void	zoom_in(t_fractol *fract, double mouse_r, double mouse_i)
{
	double zoomFactor;

	zoomFactor = 0.95;
	fract->shift_r += (mouse_r - fract->shift_r) * (1 - zoomFactor);
	fract->shift_i += (mouse_i - fract->shift_i) * (1 - zoomFactor);
	fract->zoom *= zoomFactor;
}

void	zoom_out(t_fractol *fract, double mouse_r, double mouse_i)
{
	double zoomFactor;

	zoomFactor = 1.01;
	fract->shift_r += (mouse_r - fract->shift_r) * (1 - zoomFactor);
	fract->shift_i += (mouse_i - fract->shift_i) * (1 - zoomFactor);
	fract->zoom *= zoomFactor;	
}