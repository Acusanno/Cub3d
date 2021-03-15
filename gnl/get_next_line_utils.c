/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 12:57:05 by acusanno          #+#    #+#             */
/*   Updated: 2020/12/17 12:46:43 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		ft_strlen(const char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *prefixe, char const *suffixe)
{
	char	*concat;
	int		len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!prefixe && !suffixe)
		return (NULL);
	len = ft_strlen(prefixe) + ft_strlen(suffixe);
	if (!(concat = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (i < ft_strlen(prefixe))
	{
		concat[i] = prefixe[i];
		i++;
	}
	while (j < ft_strlen(suffixe))
	{
		concat[i + j] = suffixe[j];
		j++;
	}
	concat[len] = 0;
	free((char *)prefixe);
	return (concat);
}

int		ft_isline_break(const char *s)
{
	int i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t i;

	i = 0;
	if (src == 0)
		return (0);
	while (src[i] && i < dstsize - 1)
	{
		if (dstsize > 0)
		{
			dst[i] = src[i];
		}
		i++;
	}
	if (dstsize > 0)
		dst[i] = 0;
	while (src[i])
		i++;
	return (dst);
}

char	*ft_substr(char const *s, int start, size_t len, int *ret2)
{
	char			*str;
	unsigned int	i;

	i = 0;
	if (!(str = malloc(sizeof(char) * len + 1)))
	{
		*ret2 = -1;
		return (0);
	}
	if (s == 0)
		return (0);
	if (start >= ft_strlen(s))
	{
		str[i] = 0;
		return (str);
	}
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (&str[0]);
}
