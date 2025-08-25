#include "../include/cub3d.h"

/* prosta kontrola ściany w mapie-kwadratach (cells) */
static int	is_wall(t_scene *sc, int mx, int my)
{
	if (mx < 0 || my < 0 || mx >= sc->w || my >= sc->h)
		return (1);
	return (sc->map[my][mx] == '1');
}

static unsigned int	texel_at(t_texture *t, int tx, int ty)
{
	int	i;

	if (tx < 0) tx = 0; if (tx >= t->w) tx = t->w - 1;
	if (ty < 0) ty = 0; if (ty >= t->h) ty = t->h - 1;
	i = ty * t->line_len + tx * (t->bpp / 8);
	return ((unsigned char)t->data[i]
		| ((unsigned char)t->data[i + 1] << 8)
		| ((unsigned char)t->data[i + 2] << 16));
}

void	cast_and_draw_all(t_game *g)
{
	const float	fov = 60.0f * PI_VAL / 180.0f;
	const float	dir0 = g->pl.dir - fov / 2.0f;
	const float	step_ang = fov / (float)W_WIDTH;

	const float	posX = g->pl.x / (float)TILE; /* pozycja w jednostkach pól */
	const float	posY = g->pl.y / (float)TILE;

	int	x = 0;
	while (x < W_WIDTH)
	{
		float	ray_ang = dir0 + step_ang * (float)x;
		float	ray_dx = cosf(ray_ang);
		float	ray_dy = sinf(ray_ang);

		/* DDA setup (wszystko w jednostkach pól) */
		int		mapX = (int)posX;
		int		mapY = (int)posY;
		float	sideDistX;
		float	sideDistY;
		float	deltaDistX = (ray_dx == 0.0f) ? 1e30f : fabsf(1.0f / ray_dx);
		float	deltaDistY = (ray_dy == 0.0f) ? 1e30f : fabsf(1.0f / ray_dy);
		int		stepX = (ray_dx < 0.0f) ? -1 : 1;
		int		stepY = (ray_dy < 0.0f) ? -1 : 1;

		float	posXfrac = posX - floorf(posX);
		float	posYfrac = posY - floorf(posY);

		if (ray_dx < 0.0f)
			sideDistX = posXfrac * deltaDistX;
		else
			sideDistX = (1.0f - posXfrac) * deltaDistX;
		if (ray_dy < 0.0f)
			sideDistY = posYfrac * deltaDistY;
		else
			sideDistY = (1.0f - posYfrac) * deltaDistY;

		int		side = -1; /* 0: trafienie na ścianie pionowej (E/W), 1: poziomej (N/S) */
		while (1)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (is_wall(&g->sc, mapX, mapY))
				break ;
		}

		/* odległość prostopadła do kamery (NIE euklidesowa!) w jednostkach pól */
		float	perp;
		if (side == 0)
			perp = (mapX - posX + (stepX < 0 ? 1.0f : 0.0f)) / ray_dx;
		else
			perp = (mapY - posY + (stepY < 0 ? 1.0f : 0.0f)) / ray_dy;

		/* wysokość kolumny – zależy od WYSOKOŚCI okna, nie szerokości */
		int		line_h = (int)(W_HEIGHT / perp);
		int		draw_start = -line_h / 2 + W_HEIGHT / 2;
		int		draw_end = line_h / 2 + W_HEIGHT / 2;
		if (draw_start < 0) draw_start = 0;
		if (draw_end >= W_HEIGHT) draw_end = W_HEIGHT - 1;

		/* współrzędna „u” trafienia na ścianie (0..1), w jednostkach pól */
		float	wallx;
		if (side == 0)
			wallx = posY + perp * ray_dy;
		else
			wallx = posX + perp * ray_dx;
		wallx -= floorf(wallx);

		/* wybór tekstury po stronie świata + ewentualne odwrócenie texX */
		t_texture	*tex;
		if (side == 0 && stepX < 0) tex = &g->tex_we; /* zachód */
		else if (side == 0)         tex = &g->tex_ea; /* wschód */
		else if (side == 1 && stepY < 0) tex = &g->tex_no; /* północ */
		else                        tex = &g->tex_so; /* południe */

		int		texX = (int)(wallx * (float)tex->w);
		/* odwróć kolumnę tekstury, żeby orientacja była poprawna */
		if (side == 0 && ray_dx > 0.0f)  texX = tex->w - texX - 1;
		if (side == 1 && ray_dy < 0.0f)  texX = tex->w - texX - 1;

		/* stały krok w teksturze na każdy piksel kolumny → brak „rozciągania” */
		float	step = (float)tex->h / (float)line_h;
		float	texPos = (draw_start - (W_HEIGHT / 2 - line_h / 2)) * step;

		int	y = draw_start;
		while (y <= draw_end)
		{
			int texY = (int)texPos;
			unsigned int c = texel_at(tex, texX, texY);
			/* (opcjonalnie) lekkie przyciemnienie ścian „side==1” dla lepszego odbioru:
			   if (side==1) c = ((c & 0xFEFEFE) >> 1); */
			put_pixel(x, y, c, g);
			texPos += step;
			y++;
		}
		x++;
	}
}
