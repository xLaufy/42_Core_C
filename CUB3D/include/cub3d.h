/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:37:01 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/09/13 18:47:14 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <math.h>
# include "../minilibx-linux/mlx.h"
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>

# define W_WIDTH 1200
# define W_HEIGHT 600
# define TILE 64
# define PI_VAL 3.14159265358979323846

# define KEY_W 119
# define KEY_S 115
# define KEY_A 97
# define KEY_D 100
# define KEY_L 65361
# define KEY_R 65363
# define KEY_ESC 65307

typedef struct s_texture
{
	void		*img;
	char		*data;
	int			bpp;
	int			endian;
	int			line_len;
	int			w;
	int			h;
}				t_texture;

typedef struct s_player
{
	float		x;
	float		y;
	float		dir;
	bool		k_w;
	bool		k_s;
	bool		k_a;
	bool		k_d;
	bool		k_l;
	bool		k_r;
}				t_player;

typedef struct s_scene
{
	char		*no;
	char		*so;
	char		*we;
	char		*ea;
	int			f_rgb;
	int			c_rgb;
	char		**map;
	int			w;
	int			h;
	char		spawn;
}				t_scene;

typedef struct s_cast
{
	int			map_x;
	int			map_y;
	int			side;
	int			step_x;
	int			step_y;
	float		cs;
	float		sn;
	float		side_dx;
	float		side_dy;
	float		dx;
	float		dy;
	float		perp;
	float		wallx;
	int			top;
	int			bot;
	int			tex_x;
}				t_cast;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*data;
	int			bpp;
	int			endian;
	int			line_len;
	t_scene		sc;
	t_player	pl;
	t_texture	tex_no;
	t_texture	tex_so;
	t_texture	tex_we;
	t_texture	tex_ea;
}				t_game;

static void	init_ray_params(t_game *g, int x, t_cast *cast, float dir0,
		float step_ang);
static void	perform_dda(t_game *g, t_cast *cast);
static void calculate_distance_and_height(t_game *g, t_cast *cast, float posX, float posY);
static t_texture *select_texture_and_coords(t_game *g, t_cast *cast, float posX, float posY);
static void draw_wall_strip(t_game *g, int x, t_cast *cast, t_texture *tex);
void cast_and_draw_all(t_game *g);

void			init_game(t_game *g, const char *cub_path);
int				close_game(t_game *g);

int				draw_loop(t_game *g);
int				key_press(int k, t_game *g);
int				key_release(int k, t_game *g);

void			parse_cub(const char *path, t_scene *sc);
void			free_scene(t_scene *sc);
void			load_textures(t_game *g);
void			destroy_textures(t_game *g);

void			cast_and_draw_all(t_game *g);

void			put_pixel(int x, int y, int color, t_game *g);

void			ft_free_split(char **arr);
char			**load_lines(const char *path);
int				set_id(t_scene *sc, char *line);

#endif
