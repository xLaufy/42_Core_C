#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	if (c == 0)
		return ((char *)s + i);
	while (i >= 0)
	{
		if (s[i] == c)
			return ((char *)s + i);
		i--;
	}
	return (NULL);
}
// 7. Finding length of string s

// 8. If c = '\0', we return pointer to the end of the string

// 12. Iterating backwards throught the string in search for last c occurence

// 16. Return NULL if c is nowhere to find
