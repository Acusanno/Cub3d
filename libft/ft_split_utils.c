#include "libft.h"

int	countword(char const *s, char c)
{
	int	i;

	i = 0;
	while (*s)
	{
		{
			if (*s && *s != c && (*(s + 1) == '\0' || *(s + 1) == c))
				i++;
			s++;
		}
	}
	return (i);
}

char	**free_split(char **str_tot, int i)
{
	while (--i >= 0)
		free(str_tot[i]);
	free(str_tot);
	return (NULL);
}
