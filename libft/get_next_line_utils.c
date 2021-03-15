/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 12:57:05 by acusanno          #+#    #+#             */
/*   Updated: 2021/02/24 10:22:53 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strlen_gnl(const char *str)
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
	len = ft_strlen_gnl(prefixe) + ft_strlen_gnl(suffixe);
	if (!(concat = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (i < ft_strlen_gnl(prefixe))
	{
		concat[i] = prefixe[i];
		i++;
	}
	while (j < ft_strlen_gnl(suffixe))
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

char	*ft_strlcpy_gnl(char *dst, const char *src, size_t dstsize)
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

char	*ftgsubstr(char const *s, int start, size_t len, int *ret2)
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
	if (start >= ft_strlen_gnl(s))
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
