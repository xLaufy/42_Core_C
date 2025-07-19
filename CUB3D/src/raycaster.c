#include "../include/game.h"

bool touch(float px, float py, t_game *game)
{
    int x;
    int y;

    x = px / BLOCK;
    y = py / BLOCK;
    if (game->map[y][x] == '1')
        return (true);
    return (false);
}

float distance(float x, float y)
{
    return (sqrt(x * x + y * y));
}

float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
    float delta_x = x2 - x1;
    float delta_y = y2 - y1;
    float fov = atan2(delta_y, delta_x) - game->player.fov;
    float fix_dist = distance(delta_x, delta_y) * cos(fov);
    return fix_dist;
}

void draw_line(t_player *player, t_game *game, float start_x, int i)
{
    float cos_angle;
	float sin_angle;
    float ray_x;
    float ray_y;
    float dist;
    float wall_height;
    int start_y;
    int end;
    int floor;
    int ceiling;

    cos_angle = cos(start_x);
	sin_angle = sin(start_x);
    ray_x = player->x;
    ray_y = player->y;

    while (!touch(ray_x, ray_y, game))
    {
        ray_x += cos_angle;
        ray_y += sin_angle;
    }

    dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
    wall_height = (BLOCK / dist) * (WIDTH / 2);
    start_y = (HEIGHT - wall_height) / 2;
    end = start_y + wall_height;
    floor = (HEIGHT - wall_height) / 2;
    ceiling = start_y;
    
    while (ceiling > 0)
    {
        put_pixel(i, ceiling, 0x66B2FF, game);
        ceiling--;
    }
    
    while (floor < HEIGHT)
    {
        put_pixel(i, floor, 0xE0E0E0, game);
        floor++;
    }

    while (start_y < end)
    {
        put_pixel(i, start_y, 0x663300, game);
        start_y++;
    }
}
