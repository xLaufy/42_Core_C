#include "../include/game.h"

void put_pixel(int x, int y, int color, t_game *game)
{
	if (x >= WIDTH || y >= HEIGHT || x <= 0 || y <= 0)
		return;
	int index = y * game->line_len + x * game->bpp / 8;
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;

}	

void init_game(t_game *game)
{
	game->map = get_map();
	if (!game->map) {
        fprintf(stderr, "Error: Map initialization failed\n");
        exit(EXIT_FAILURE);
    }
	init_player(&game->player);
	game->mlx_ptr = mlx_init();
	if (!game->mlx_ptr) {
        fprintf(stderr, "Error: MLX initialization failed\n");
        exit(EXIT_FAILURE);
    }
	game->win_ptr = mlx_new_window(game->mlx_ptr, WIDTH, HEIGHT, "CUB3D");
	 if (!game->win_ptr) {
        fprintf(stderr, "Error: Window creation failed\n");
        exit(EXIT_FAILURE);
    }
	game->img_ptr = mlx_new_image(game->mlx_ptr, WIDTH, HEIGHT);
	if (!game->img_ptr) {
        fprintf(stderr, "Error: Image creation failed\n");
        exit(EXIT_FAILURE);
    }
	game->data = mlx_get_data_addr(game->img_ptr, &game->bpp, &game->line_len, &game->endian);
	 if (!game->data) {
        fprintf(stderr, "Error: Data address retrieval failed\n");
        exit(EXIT_FAILURE);
    }
	
	game->width    = WIDTH;
    game->height   = HEIGHT;
	
	mlx_put_image_to_window (game->mlx_ptr, game->win_ptr, game->img_ptr, 0, 0);
}
