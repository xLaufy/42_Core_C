#include "../include/game.h"

int draw_loop(t_game *game)
{
	size_t buffer_size;
	float angle_step;
	float initial_angle;
	int column_index;
	t_player *player;
	
	buffer_size = (size_t)game->height * (size_t)game->line_len;\
	player = &game->player;
	angle_step = PI / 3 / WIDTH;
	initial_angle = game->player.fov - PI / 6;
	column_index = 0;

	ft_bzero(game->data, buffer_size);
	player_move(&game->player);

	while (column_index < WIDTH)
	{
		draw_line(player, game, initial_angle, column_index);
		initial_angle += angle_step;
		column_index++;
	}
	
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->img_ptr, 0, 0);
	return (0);
}

int main(void)
{
 	t_game	game;

	init_game(&game);

	mlx_hook(game.win_ptr, 2, 1L<<0, key_press, &game.player);
	mlx_hook(game.win_ptr, 3, 1L<<1, key_release, &game.player);

	mlx_loop_hook(game.mlx_ptr, draw_loop, &game);

	mlx_loop(game.mlx_ptr);

	mlx_destroy_window(game.mlx_ptr, game.win_ptr);
	return (0);
}


// gcc cub3d.c -Lminilibx-linux -lmlx_Linux -lX11 -lXext
// export DISPLAY=$(awk '/nameserver / {print $2; exit}' /etc/resolv.conf 2>/dev/null):0
