# Struktura projektu so_long

```
so_long/
├── Makefile
├── include/
│   ├── so_long.h
│   ├── structs.h
│   └── constants.h
├── src/
│   ├── main.c
│   ├── init/
│   │   ├── init_game.c
│   │   └── load_sprites.c
│   ├── map/
│   │   ├── parse_map.c
│   │   ├── fill_grid.c
│   │   └── validate_map.c
│   ├── render/
│   │   ├── render.c
│   │   ├── draw_background.c
│   │   ├── draw_map.c
│   │   └── draw_player.c
│   ├── events/
│   │   ├── key_handler.c
│   │   └── update_player_position.c
│   ├── utils/
│   │   ├── error_handling.c
│   │   └── memory_management.c
│   └── game_logic/
│       ├── check_win_condition.c
│       └── collect_item.c
├── maps/
│   ├── level1.ber
│   └── level2.ber
├── img/
│   ├── player.xpm
│   ├── wall.xpm
│   ├── collectible.xpm
│   └── exit.xpm
└── libft/
    └── (pliki biblioteki libft)
```

## Kolejność implementacji funkcji:

1. Rozpocznij od plików nagłówkowych:
   - `include/so_long.h`: Główny plik nagłówkowy
   - `include/structs.h`: Definicje struktur
   - `include/constants.h`: Stałe i makra

2. Implementacja podstawowych funkcji:
   - `src/main.c`: Główna funkcja programu
   - `src/init/init_game.c`: Inicjalizacja gry
   - `src/utils/error_handling.c`: Obsługa błędów

3. Parsowanie i walidacja mapy:
   - `src/map/parse_map.c`: Wczytywanie mapy z pliku
   - `src/map/fill_grid.c`: Wypełnianie siatki mapy
   - `src/map/validate_map.c`: Sprawdzanie poprawności mapy

4. Renderowanie:
   - `src/render/render.c`: Główna funkcja renderująca
   - `src/render/draw_background.c`: Rysowanie tła
   - `src/render/draw_map.c`: Rysowanie elementów mapy
   - `src/render/draw_player.c`: Rysowanie gracza

5. Obsługa zdarzeń:
   - `src/events/key_handler.c`: Obsługa klawiszy
   - `src/events/update_player_position.c`: Aktualizacja pozycji gracza

6. Logika gry:
   - `src/game_logic/check_win_condition.c`: Sprawdzanie warunku zwycięstwa
   - `src/game_logic/collect_item.c`: Zbieranie przedmiotów

7. Zarządzanie zasobami:
   - `src/init/load_sprites.c`: Ładowanie sprite'ów
   - `src/utils/memory_management.c`: Zarządzanie pamięcią

8. Makefile: Skonfiguruj Makefile do kompilacji projektu

9. Testowanie i debugowanie: Systematycznie testuj każdą zaimplementowaną funkcję

10. Optymalizacja i rozszerzenia (opcjonalnie):
    - Dodawanie przeciwników
    - System poziomów
    - System punktacji
    - Efekty dźwiękowe

Pamiętaj, aby regularnie kompilować i testować kod podczas implementacji. Zacznij od podstawowych funkcjonalności i stopniowo dodawaj bardziej zaawansowane elementy.
