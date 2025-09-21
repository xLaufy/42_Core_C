/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkobelie <rkobelie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:37:01 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/09/21 21:41:19 by rkobelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <math.h>
# include <mlx.h>
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
	void			*img;
	char			*data;
	int				bpp;
	int				endian;
	int				line_len;
	int				w;
	int				h;
}					t_texture;

typedef struct s_input
{
	float			mv;
	float			rt;
	float			cs;
	float			sn;
	float			new_x;
	float			new_y;

}					t_input;

typedef struct s_colon
{
	float			margin;
	int				map_x;
	int				map_y;
	float			frac_x;
	float			frac_y;

}					t_colon;

typedef struct s_parse_id
{
	char			*s;
	char			*key;
	char			*val;
	int				ok;

}					t_parse_id;

typedef struct s_spfs
{
	int				y;
	int				x;
	char			c;
	float			min_distance;

}					t_spfs;

typedef struct s_player
{
	float			x;
	float			y;
	float			dir;
	bool			k_w;
	bool			k_s;
	bool			k_a;
	bool			k_d;
	bool			k_l;
	bool			k_r;
}					t_player;

typedef struct s_scene
{
	char			*no;
	char			*so;
	char			*we;
	char			*ea;
	int				f_rgb;
	int				c_rgb;
	char			**map;
	int				w;
	int				h;
	char			spawn;
}					t_scene;

typedef struct s_cast
{
	float			fov;
	float			dir0;
	float			step_ang;
	float			posX;
	float			posY;
	int				x;
	float			ray_ang;
	float			ray_dx;
	float			ray_dy;
	int				mapX;
	int				mapY;
	float			sideDistX;
	float			sideDistY;
	float			deltaDistX;
	float			deltaDistY;
	int				stepX;
	int				stepY;
	float			posXfrac;
	float			posYfrac;
	int				side;
	float			perp;
	int				line_h;
	int				draw_start;
	int				draw_end;
	float			wallx;
	t_texture		*tex;
	int				tex_x;
	float			step;
	float			texPos;
	int				y;
	int				texY;
	unsigned int	c;
}					t_cast;

typedef struct s_game
{
	void			*mlx;
	void			*win;
	void			*img;
	char			*data;
	int				bpp;
	int				endian;
	int				line_len;
	t_scene			sc;
	t_player		pl;
	t_texture		tex_no;
	t_texture		tex_so;
	t_texture		tex_we;
	t_texture		tex_ea;
}					t_game;

int					is_wall(t_scene *sc, int mx, int my);

void				w_key(t_game *g, t_input *input);
void				s_key(t_game *g, t_input *input);
void				a_key(t_game *g, t_input *input);
void				d_key(t_game *g, t_input *input);
void				key_press_collision(t_game *g, t_input *input);
t_colon				init_colon(float new_x, float new_y);

void				dda_algorithm(t_cast *cast, t_game *g);
void				draw_vertical_line(t_cast *cast, t_game *g);
void				select_texture(t_cast *cast, t_game *g);
void				calc_tex_x(t_cast *cast);
void				calc_tex_step_and_pos(t_cast *cast);
void				calc_wallx(t_cast *cast);
void				calc_line_height_and_bounds(t_cast *cast);
int					check_collision(t_scene *sc, float new_x, float new_y);
float				calc_prep_dist(t_cast *cast);
void				calc_tex_step_and_pos(t_cast *cast);
void				select_texture(t_cast *cast, t_game *g);
void				calc_wallx(t_cast *cast);
void				delta_dist(t_cast *cast, t_game *g);
void				side_dist_acount(t_cast *cast);
void				init_steps(t_cast *cast);
void				check_spawn_and_closed_row(t_scene *sc, int y, int *cnt);
int					skip_ids_and_empty(char **ls, t_scene *sc);
int					set_id_keyval(t_scene *sc, const char *key,
						const char *val);
int					parse_rgb_num(char *s, int *out_rgb);

const char			*skip_ws(const char *s);
int					is_id_line(const char *raw);
char				*dup_no_cr(const char *ln);

void				init_game(t_game *g, const char *cub_path);
int					close_game(t_game *g);
void				init_start_values(t_cast *cast, t_game *g);
void				init_other_values(t_cast *cast, t_game *g);

int					draw_loop(t_game *g);
int					key_press(int k, t_game *g);
int					key_release(int k, t_game *g);

void				parse_cub(const char *path, t_scene *sc);
void				free_scene(t_scene *sc);
void				load_textures(t_game *g);
void				destroy_textures(t_game *g);

void				cast_and_draw_all(t_game *g);
unsigned int		texel_at(t_texture *t, int tx, int ty);
void				put_pixel(int x, int y, int color, t_game *g);

void				ft_free_split(char **arr);
char				**load_lines(const char *path);
int					set_id(t_scene *sc, char *line);

#endif
