#include "libft.h"

void	*ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*dest;

	i = 0;
	dest = s;
	while (i < n)
		dest[i++] = 0;
}
