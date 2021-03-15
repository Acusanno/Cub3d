/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 09:20:31 by acusanno          #+#    #+#             */
/*   Updated: 2020/12/02 14:52:40 by acusanno         ###   ########lyon.fr   */
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
	int i;

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
	return (0);
}

char		**ft_split(char const *s, char c)
{
	char	**str_tot;
	char	*start;
	int		i;

	i = 0;
	if (s == 0 || !(str_tot = malloc(sizeof(char *) * (countword(s, c) + 1))))
		return (0);
	while (*s)
	{
		while (*s && ft_isset(*s, c) == 1)
			s++;
		start = (char *)s;
		while (*s && ft_isset(*s, c) == 0)
			s++;
		if (s != start)
		{
			if (!(str_tot[i] = malloc(sizeof(char) * (s - start + 1))))
				return (free_split(str_tot, i));
			ft_strlcpy(str_tot[i], start, s - start + 1);
			i++;
		}
	}
	str_tot[i] = 0;
	return (str_tot);
}
