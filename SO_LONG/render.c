#include "so_long.h"

int render(t_vars *vars) 
{
    // Rysowanie tła gry
    draw_background(vars);

    // Sprawdzenie, czy gra nie została jeszcze wygrana
    if (!vars->won) 
    {
        // Rysowanie elementów mapy (ściany, przedmioty, wyjście)
        draw_map(vars);

        // Rysowanie gracza na jego aktualnej pozycji
        draw_player(vars);

        // Wyświetlanie liczby wykonanych ruchów
        draw_moves(vars);
    } 
    else
    {
        // Jeśli gra została wygrana, wyświetl ekran zwycięstwa
        mlx_put_image_to_window(vars->mlx, vars->win, vars->yw_sp.img, 0, 0);

        // Wyświetlanie końcowej liczby ruchów
        mlx_put_image_to_window(vars->mlx, vars->win, vars->digits_sp [(vars->moves / 100)].img, 1 * SIZE + 10, 3 * SIZE + 10);

        mlx_put_image_to_window(vars->mlx, vars->win, vars->digits_sp[(vars->moves / 10) % 10].img, 2 * SIZE + 10, 3 * SIZE + 10);

        mlx_put_image_to_window(vars->mlx, vars->win, vars->digits_sp       [(vars->moves % 100) % 10].img, 3 * SIZE + 10, 3 * SIZE + 10);
    }
    // Zwrócenie 0, aby kontynuować pętlę renderowania
    return (0);
}



draw_map(char *param);

    
draw_player(char *param);

  
draw_moves(char *param);