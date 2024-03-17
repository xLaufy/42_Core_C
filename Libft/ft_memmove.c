#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	const char		*s;

	d = (char *)dest;
	s = (char *)src;
	if (dest <= src)
	{
		while (n--)
		{
			*d++ = *s++;
		}
	}
	else if (dest > src)
	{
		d += n - 1;
		s += n - 1;
		while (n--)
			*d-- = *s--;
	}
	return (dest);
	if (!dest && !src)
		return (dest);
}
