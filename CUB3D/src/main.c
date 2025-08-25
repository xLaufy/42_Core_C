#include "../include/cub3d.h"

static void	handle_input(t_game *g)
{
	float	mv;
	float	rt;
	float	cs;
	float	sn;

	mv = 3.0f;
	rt = 0.06f;
	if (g->pl.k_l)
		g->pl.dir -= rt;
	if (g->pl.k_r)
		g->pl.dir += rt;
	cs = cosf(g->pl.dir);
	sn = sinf(g->pl.dir);
	if (g->pl.k_w)
	{
		g->pl.x += cs * mv;
		g->pl.y += sn * mv;
	}
	if (g->pl.k_s)
	{
		g->pl.x -= cs * mv;
		g->pl.y -= sn * mv;
	}
	if (g->pl.k_a)
	{
		g->pl.x += sn * mv;
		g->pl.y -= cs * mv;
	}
	if (g->pl.k_d)
	{
		g->pl.x -= sn * mv;
		g->pl.y += cs * mv;
	}
}

int	draw_loop(t_game *g)
{
	int		y;
	int		x;
	size_t	sz;

	sz = (size_t)g->line_len * (size_t)W_HEIGHT;
	ft_bzero(g->data, sz);
	handle_input(g);
	y = 0;
	while (y < W_HEIGHT)
	{
		x = 0;
		while (x < W_WIDTH)
		{
			if (y < W_HEIGHT / 2)
				put_pixel(x, y, g->sc.c_rgb, g);
			else
				put_pixel(x, y, g->sc.f_rgb, g);
			x++;
		}
		y++;
	}
	cast_and_draw_all(g);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	return (0);
}

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

int	main(int ac, char **av)
{
	t_game	g;

	if (ac != 2)
		return (1);
	ft_bzero(&g, sizeof(g));
	init_game(&g, av[1]);
	mlx_hook(g.win, 17, 0, close_game, &g);
	mlx_hook(g.win, 2, 1L << 0, key_press, &g);
	mlx_hook(g.win, 3, 1L << 1, key_release, &g);
	mlx_loop_hook(g.mlx, draw_loop, &g);
	mlx_loop(g.mlx);
	return (0);
}


// gcc cub3d.c -Lminilibx-linux -lmlx_Linux -lX11 -lXext
// export DISPLAY=$(awk '/nameserver / {print $2; exit}' /etc/resolv.conf 2>/dev/null):0
