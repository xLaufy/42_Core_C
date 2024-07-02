#include "so_long.h"

int parse_map(t_map *map)
{
    int fd;
    char *line;

    fd = open(map->path, O_RDONLY);
    if (fd < 0)
        map_error("Can't find the map");
    
    map->g_h = 0;
    map->g_w = 0;

    line = get_next_line(fd);

    while (line)
        map->g_h++;
    if (map.g_h  == 1)
        map->g_w = ft_line_len(line);

    // sprawdzenie, czy aktualna linia ma taką samą długość jak pierwsza
    if (ft_line_len(line) != map.g_w)
        map_error("Map is not rectangular")
    line = get_next_line(fd);
    if (map->g_h == 0)
        map_error("Empty map file");
    return (1);
}

int fill_grid(t_vars *vars)
{
    t_point g_pos;  //aktualna pozycja na siatce
    char *line;

    // Inicjalizacja zmiennych do wypełniania mapy
    initiate_map_filling(vars, &g_pos);

    // Wczytanie pierwszej linii z pliku
    line = get_next_line(vars->map.fd);

    while (line)
    {
        allocate_line(vars, g_pos);

        while (g_pos.px_x < vars->map.g_w)
        {
            // Wypełnienie kafelka danymi z wczytanej linii
            fill_tiles(vars, line, g_pos);

            // Zliczenie elementów na mapie (start, wyjście, przedmioty)
            count_grid(vars, vars->map.grid[g_pos.px_y][g_pos.px_x], g_pos);

            //przejście do następnej kolumny
            g_pos.px.x++;
        }
        //przejście do początku następnego wiersza
        g_pos.px.x = 0;
        g_pos.px.y++;

        //Wczytanie kolejnej linii
        line = get_next_line(vars->map.fd);
    }
    //zamknięcie pliku mapy
    close(vars->map.fd);

    // Sprawdzenie, czy mapa jest otoczona ścianami
    if (no_walls(vars))
        map_error("Invalid map, not surrounded by walls");
    // Sprawdzenie, czy istnieje ścieżka do wyjścia i przedmiotów
    check_path(vars->plauer.pos, vars);
    //sprawdzenie poprawności mapy
    check_map(vars);
}