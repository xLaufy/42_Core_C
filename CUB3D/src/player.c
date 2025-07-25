#include "../include/game.h"

void init_player(t_player *player)
{ 
    player->x = WIDTH / 2;
    player->y = HEIGHT / 2;
    player->fov = PI / 2;
    player->key_up = false;
    player->key_down = false;
    player->key_left = false;
    player->key_right = false;

    player->l_rotate = false;
    player->r_rotate = false;
}

int key_press(int keycode, t_player *player)
{
    if (keycode == W)
        player->key_up = true;
    if (keycode == S)
        player->key_down = true;
    if (keycode == A)
        player->key_left = true;
    if (keycode == D)
        player->key_right = true;
     if(keycode == R_LEFT)
        player->l_rotate = true;
    if(keycode == R_RIGHT)
        player->r_rotate = true;
    return (0);
}

int key_release(int keycode, t_player *player)
{
    if (keycode == W)
        player->key_up = false;
    if (keycode == S)
        player->key_down = false;
    if (keycode == A)
        player->key_left = false;
    if (keycode == D)
        player->key_right = false;
    if(keycode == R_LEFT)
        player->l_rotate = false;
    if(keycode == R_RIGHT)
        player->r_rotate = false;
    return 0;
    return (0);
}

void player_move(t_player *player)
{
    int speed;
    float angle_speed;
    float cos_angle;
    float sin_angle;


    speed = 3;
    angle_speed = 0.06;
    cos_angle = cos(player->fov);
    sin_angle = sin(player->fov);
    
    if (player->l_rotate)
        player->fov -= angle_speed;
    if (player->r_rotate)
        player->fov += angle_speed;
    if (player->fov > 2 * PI)
        player->fov = 0;
    if (player->fov < 0)
        player->fov = 2 * PI;

    if (player->key_up)
    {
        player->x += cos_angle * speed;
        player->y += sin_angle * speed;
    }
    if (player->key_down)
    {
        player->x -= cos_angle * speed;
        player->y -= sin_angle * speed;
    }
    if (player->key_left)
    {
        player->x += sin_angle * speed;
        player->y -= cos_angle * speed;
    }
    if (player->key_right)
    {
        player->x -= sin_angle * speed;
        player->y += cos_angle * speed;
    }
    
}