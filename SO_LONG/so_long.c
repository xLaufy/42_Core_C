#include "so_long.h"

typedef struct s_data
{  
    void *img;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_data;

void my_mlx_pixel_put(t_data *data , int x, int y, int color)
{
    char *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color; 

}

int main(void)
{
    void *mlx;
    void *window;
    t_data img;

    printf("Initializing MiniLibX...\n");
    mlx = mlx_init();
    if (mlx == NULL)
    {
        printf("Failed to initialize MiniLibX\n");
        return 1;
    }

    printf("Creating window...\n");
    window = mlx_new_window(mlx, 600, 300, "so_long");
    img.img = mlx_new_image(mlx, 640, 360);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
    if (window == NULL)
    {
        printf("Failed to create window\n");
        return 1;
    }

    printf("Entering event loop...\n");

    my_mlx_pixel_put(&img, 5, 5, 0x00FF0000);
    mlx_put_image_to_window(mlx, window, img.img, 0, 0);
    mlx_loop(mlx);
    mlx_destroy_window(mlx, window);
    mlx_destroy_display(mlx);
    free(mlx);
} 


// cc so_long.c -I ./minilibx-linux -L ./minilibx-linux -lmlx -lXext -lX11  -> do skompilowania pliku egzekucyjnego 