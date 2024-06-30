#include "keys.h"
#include "so_long.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

typedef struct s_data
{
	void	*img;
	char	*address;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_data;

typedef struct s_vars
{
	void	*mlx;
	void	*window;
}			t_vars;


void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*pixel;

	if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
	{
		pixel = data->address + (y * data->line_length + x
				* (data->bits_per_pixel / 8));
		*(unsigned int *)pixel = color;
	}
}
// hook for kclosing the window by pressing esc
int	close_window(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->window);
	exit (0);
}

int key_handler(int keycode, t_vars *vars)
{
    if (keycode == K_ESC)
	{
        printf("ESCAPE");
		close_window(vars);
	}
    else if (keycode == 0 || keycode == 123)
        printf("LEFT (A / ARROW_LEFT)");
    else if (keycode == 2 || keycode == 124)
        printf("RIGHT (D / ARROW_RIGHT)");
    else if (keycode == 1 || keycode == 125)
        printf("DOWN (S / ARROW_DOWN)");
    else if (keycode == 13 || keycode == 126)
        printf("LEFT (A / ARROW_LEFT)");
    else
        printf("%d\n", keycode);
    return (0);
}
// int key_handler(int keycode, t_vars *vars)
// {
// 	if (keycode == K_ESC || keycode == 65307)
// 		close(vars);
// 	else if (keycode == K_A || keycode == K_AR_L)
// 		update_player_position(vars, (t_point){vars->player.pos.px_x - 1, vars->player.pos.px_y});
// 	else if (keycode == K_D || keycode == K_AR_R)
// 		update_player_position(vars, (t_point){vars->player.pos.px_x + 1, vars->player.pos.px_y});
// 	else if (keycode == K_W || keycode == K_AR_U)
// 		update_player_position(vars, (t_point){vars->player.pos.px_x, vars->player.pos.px_y + 1});
// 	else if (keycode == K_S || keycode == K_AR_D)
// 		update_player_position(vars, (t_point){vars->player.pos.px_x, vars->player.pos.px_y - 1});
// 	return (0);
// }
// hook prototype
// void mlx_hook(mlx_win_list_t *win_ptr, int x_event, int x_mask, int (*f)(),
//	void *param)

int	main(void)
{
	t_vars	vars;

	// t_data	img;
	printf("Initializing MiniLibX...\n");
	vars.mlx = mlx_init();
	if (vars.mlx == NULL)
	{
		printf("Failed to initialize MiniLibX\n");
		return (1);
	}
	printf("Creating window...\n");
	vars.window = mlx_new_window(vars.mlx, WINDOW_WIDTH, WINDOW_HEIGHT,
			"so_long");
	// img.img = mlx_new_image(mlx, 640, 360);
	// img.address = mlx_get_data_addr(img.img, &img.bits_per_pixel,
	//		&img.line_length,
	//		&img.endian);
	if (vars.window == NULL)
	{
		printf("Failed to create window\n");
		return (1);
	}
	printf("Entering event loop...\n");
	// my_mlx_pixel_put(&img, 5, 5, 0x00FF0000);
	// mlx_put_image_to_window(vars.mlx, window, img.img, 0, 0);
	mlx_hook(vars.window, 2, 1L << 0, key_handler, &vars);
	mlx_hook(vars.window, 17, 1L << 0, close_window, &vars);
	mlx_loop(vars.mlx);
}

//  cc so_long.c -I ./minilibx-linux -L ./minilibx-linux -lmlx -lXext -lX11
//  ->do skompilowania pliku egzekucyjnego
