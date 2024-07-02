#include "so_long.h"

// Inicjalizacja gry

void init_game(t_vars	*vars, char *map_path)
{
	vars->map.path=map_path;
	vars->collected = 0;
	vars->exit_unlocked = FALSE;
	vars->map.exit_accessible = FALSE;
	vars->map.accessible_collectibles = 0;
	vars->collectibles = 0;
	vars->won = FALSE;
	vars->moves = 0;
	vars->exit_found = 0;
	vars->start_found = 0;
}


// Główna pętla gry

int main(int ac, char **av)
{
	t_vars vars;

	if (ac != 2)
		map_error("Wrong number of arguments.");
	
	init_game(&vars, av[1]);
	parse_map(&vars.map);
	fill_grid(&vars);

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, vars.map.g_w * SIZE, vars.map.g_h * SIZE, WIN_NAME);
	load_sprites(&vars);
	mlx_hook(vars.win, 2, 1L << 0, key_handler, &vars)
	mlx_hook(vars.win, 17, 1l << 0, close_window, &vars);
	mlx_loop_hook(vars.mlx, render, &vars);

	mlx_loop(vars.mlx);
}