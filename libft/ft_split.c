/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 09:20:31 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/31 08:42:48 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	ft_isset(char c, char set)
{
	if (c == set)
		return (1);
	return (0);
}

static	int	countword(char const *s, char c)
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

static char	**free_split(char **str_tot, int i)
{
	while (--i >= 0)
		free(str_tot[i]);
	free(str_tot);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**str_tot;
	char	*start;
	int		i;

	i = 0;
	str_tot = malloc(sizeof(char *) * (countword(s, c) + 1));
	if (!s || !(str_tot))
		return (NULL);
	while (*s)
	{
		while (*s && ft_isset(*s, c) == 1)
			s++;
		start = (char *)s;
		while (*s && ft_isset(*s, c) == 0)
			s++;
		if (s != start)
		{
			(str_tot[i] = malloc(sizeof(char) * (s - start + 1)));
			if (!(str_tot[i]))
				return (free_split(str_tot, i));
			ft_strlcpy(str_tot[i++], start, s - start + 1);
		}
	}
	str_tot[i] = 0;
	return (str_tot);
}
