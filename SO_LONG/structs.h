#ifndef STRUCTS_H
# define STRUCTS_H

//pozycja na mapie
typedef struct s_point {
size_t px_x;
size_t px_y;
} t_point;

//informacje o sprite
typedef struct s_sprite {
int px_w;
int px_h;
void *img;
} t_sprite;

//informacje o graczu
typedef struct s_player {
t_point pos;
t_point start_pos;
t_sprite sprite;
} t_player;

//informacje o pojedynczym kafelku
typedef struct s_tile {
char t;
int v;
} t_tile;

//informacje o mapie
typedef struct s_map {
char *path;
size_t g_h;
size_t g_w;
char **grid;
t_tile **tiles;
int exit_accessible;
int fd;
int accessible_collectibles;
} t_map;

//główna struktura przechowująca wszystkie zmienne stanu gry
typedef struct s_vars {
void *mlx;
void *win;
void *img;
char *addr;
t_player player;
t_map map;
t_sprite w_sp;
t_sprite c_sp;
t_sprite s_sp;
t_sprite e_sp;
t_sprite f_sp;
t_sprite p_sp;
t_sprite yw_sp;
t_sprite *digits_sp;
BOOL won;
int moves;
int collected;
int collectibles;
BOOL exit_unlocked;
int exit_found;
int start_found;
int bits_per_pixel;
int line_length;
int endian;
} t_vars;

#endif