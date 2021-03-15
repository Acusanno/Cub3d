/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 18:08:29 by acusanno          #+#    #+#             */
/*   Updated: 2020/11/25 13:11:52 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	if (s[i] == c)
		return (char *)&s[i];
	i--;
	while (i >= 0)
	{
		if (s[i] == c)
			return (char *)&s[i];
		i--;
	}
	return (0);
}
