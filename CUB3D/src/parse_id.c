#include "../include/cub3d.h"
#include <fcntl.h>   /* open, O_RDONLY */
#include <unistd.h>  /* read, close */

/* pomoc: zwolnij split (jeśli nie masz w libft) */
void    ft_free_split(char **arr);

/* --- wczytywanie linii z pliku .cub --- */

static char *read_all(const char *p)
{
    int     fd;
    ssize_t rb;
    char    buf[1024];
    char    *all;
    char    *tmp;

    all = ft_strdup("");
    fd = open(p, O_RDONLY);
    if (fd < 0)
        return (NULL);
    rb = read(fd, buf, 1023);
    while (rb > 0)
    {
        buf[rb] = '\0';
        tmp = all;
        all = ft_strjoin(all, buf);
        free(tmp);
        rb = read(fd, buf, 1023);
    }
    close(fd);
    return (all);
}

char    **load_lines(const char *path)
{
    char    *all;
    char    **ls;

    all = read_all(path);
    if (!all)
        return (NULL);
    ls = ft_split(all, '\n');
    free(all);
    return (ls);
}

/* --- parsowanie F/C: "R,G,B" -> 0xRRGGBB --- */

static int  parse_rgb_num(char *s, int *out_rgb)
{
    char    **v;
    int     r;
    int     g;
    int     b;

    v = ft_split(s, ',');
    if (!v || !v[0] || !v[1] || !v[2] || v[3])
    {
        ft_free_split(v);
        return (0);
    }
    r = ft_atoi(v[0]);
    g = ft_atoi(v[1]);
    b = ft_atoi(v[2]);
    ft_free_split(v);
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        return (0);
    *out_rgb = ((r & 255) << 16) | ((g & 255) << 8) | (b & 255);
    return (1);
}

/* --- set_id: NO/SO/WE/EA/F/C --- */

int set_id(t_scene *sc, char *line)
{
	char	*s;
	int		i;
	char	*key;
	char	*val;
	int		ok;

	s = ft_strtrim(line, " \t\r\n");
	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] != ' ' && s[i] != '\t')
		i++;
	key = ft_substr(s, 0, i);
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	val = ft_strdup(s + i);
	free(s);
	if (!key || !val)
	{
		free(key);
		free(val);
		return (0);
	}
	ok = 1;
	if (!ft_strncmp(key, "NO", 3))
		sc->no = ft_strdup(val);
	else if (!ft_strncmp(key, "SO", 3))
		sc->so = ft_strdup(val);
	else if (!ft_strncmp(key, "WE", 3))
		sc->we = ft_strdup(val);
	else if (!ft_strncmp(key, "EA", 3))
		sc->ea = ft_strdup(val);
	else if (!ft_strncmp(key, "F", 2))
		ok = parse_rgb_num(val, &sc->f_rgb);
	else if (!ft_strncmp(key, "C", 2))
		ok = parse_rgb_num(val, &sc->c_rgb);
	else
		ok = 0;
	free(key);
	free(val);
	return (ok);
}

