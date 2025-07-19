#ifndef CUB3D_H
# define CUB3D_H

# include <mlx.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>
# include "../libft/libft.h"

# define WIDTH	1200
# define HEIGHT	600
# define BLOCK  60

# define W		 119
# define S		 115
# define A 		 97
# define D 		 100
# define R_LEFT	 65361
# define R_RIGHT 65363

# define PI		3.14159265358979323846

typedef struct s_player
{
	float x;
	float y;
	float fov;
	
	bool key_up;
	bool key_down;
	bool key_left;
	bool key_right;
	bool l_rotate;
	bool r_rotate;
}	t_player;

typedef struct s_game
{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img_ptr;
	int		width;
	int		height;
	char	*data;
	int		bpp;
	int		endian;
	int		line_len;
	char	**map;
	t_player player; 
}		t_game;



// typedef struct s_img
// {
// 	t_game	game;
// 	void	*img_ptr;
// 	char	*addr;
// 	int		h;
// 	int		w;
// 	int		bpp;
// 	int		endian;
// 	int		line_len;
// }		t_img;


void init_game(t_game *game);
void init_player(t_player *player);
// t_game	new_window(int w, int h, char *str);
// t_img	new_img(int w, int h, t_game window);
// void	put_pixel_img(t_img img, int x, int y, int color);
void	put_pixel(int x, int y, int color, t_game *game);
void    draw_square(int x, int y, int size, int color, t_game *game);
int key_release(int keycode, t_player *player);
int key_press(int keycode, t_player *player);
void player_move(t_player *player);
void draw_map(t_game *game);
char **get_map(void);
bool touch(float px, float py, t_game *game);
void draw_line(t_player *player, t_game *game, float start_x, int i);
float distance(float x, float y);

#endif
