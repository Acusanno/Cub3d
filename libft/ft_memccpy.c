/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 09:46:26 by acusanno          #+#    #+#             */
/*   Updated: 2020/11/25 11:16:23 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char	character;
	size_t			i;
	unsigned char	*strdst;
	unsigned char	*strsrc;

	i = 0;
	character = (unsigned char)c;
	strdst = (unsigned char *)dst;
	strsrc = (unsigned char *)src;
	while (i < n)
	{
		strdst[i] = strsrc[i];
		if (strsrc[i] == character)
			return (&strdst[i + 1]);
		i++;
	}
	return (0);
}
