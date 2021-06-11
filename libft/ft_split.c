/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 09:20:31 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/31 10:06:32 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	**split_loop(char const **s, char c, char **str_tot)
{
	int		i;
	char	*start;

	i = 0;
	while (**s)
	{
		while (**s && **s == c)
			(*s)++;
		start = (char *)(*s);
		while (**s && **s != c)
			(*s)++;
		if (*s != start)
		{
			(str_tot[i] = malloc(sizeof(char) * (*s - start + 1)));
			if (!(str_tot[i]))
				return (free_split(str_tot, i));
			ft_strlcpy(str_tot[i++], start, *s - start + 1);
		}
	}
	str_tot[i] = 0;
	return (str_tot);
}

char	**ft_split(char const *s, char c)
{
	char	**str_tot;

	if (!s)
		return (NULL);
	str_tot = malloc(sizeof(char *) * (countword(s, c) + 1));
	if (!s || !(str_tot))
		return (NULL);
	return (split_loop(&s, c, str_tot));
}

static char	**ft_split_once_2(char const *s, char c, char **str_tot)
{
	int	i;

	i = 0;
	while (*s && *s == c)
		s++;
	while (s[i] && s[i] != c)
		i++;
	if (s[i] != *s)
	{
		str_tot[0] = malloc(sizeof(char) * (i + 1));
		if (!(str_tot[0]))
			return (free_split(str_tot, 0));
		ft_strlcpy(str_tot[0], s, i + 1);
	}
	s += i;
	while (*s && *s == c)
		s++;
	if (*s)
	{
		str_tot[1] = malloc(sizeof(char) * (ft_strlen(s) + 1));
		if (!(str_tot[1]))
			return (free_split(str_tot, 1));
		ft_strlcpy(str_tot[1], s, (ft_strlen(s) + 1));
	}
	return (str_tot);
}

char	**ft_split_once(char const *s, char c)
{
	char	**str_tot;

	str_tot = malloc(sizeof(char *) * 3);
	if (!s || !(str_tot))
		return (NULL);
	if (*s)
	{
		str_tot = ft_split_once_2(s, c, str_tot);
		if (!str_tot)
			return (NULL);
	}
	str_tot[2] = 0;
	return (str_tot);
}
