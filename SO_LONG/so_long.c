#include "so_long.h"

typedef struct s_data
{  
    void *mlx;
    void *window;
} t_data;

int on_destroy(t_data *data)
{
    mlx_destroy_window(data->mlx, data->window);
    mlx_destroy_display(data->mlx);
    free(data->mlx);
    exit(0);
    return (0);
}

int on_key_press(int keysym, t_data *data)
{
    (void)data;
    printf("Key pressed: %d\n", keysym);
    return (0);
}

void *mlx_xpm_file_to_image(void *mlx_ptr, char *filename, int *width, int *height)
{
    
}

int main(void)
{
    t_data data;

    printf("Initializing MiniLibX...\n");
    data.mlx = mlx_init();
    if (data.mlx == NULL)
    {
        printf("Failed to initialize MiniLibX\n");
        return (1);
    }

    printf("Creating window...\n");
    data.window = mlx_new_window(data.mlx, 600, 300, "so_long");
    if (data.window == NULL)
    {
        printf("Failed to create window\n");
        return (free(data.mlx),1);
    }

    //Register key releaase hook
    mlx_hook(data.window, KeyRelease, KeyReleaseMask, &on_key_press, &data);

    //Register destroy hook
    mlx_hook(data.window, DestroyNotify, StructureNotifyMask, &on_destroy, &data);

    printf("Entering event loop...\n");    
    mlx_loop(data.mlx);
    return (0);
} 


// cc so_long.c -I ./minilibx-linux -L ./minilibx-linux -lmlx -lXext -lX11  -> do skompilowania pliku egzekucyjnego 