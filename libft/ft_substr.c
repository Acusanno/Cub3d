/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 17:40:51 by acusanno          #+#    #+#             */
/*   Updated: 2020/11/26 11:28:37 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include <string.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*str;
	unsigned int	i;

	i = 0;
	if (!(str = malloc(sizeof(char) * len + 1)))
		return (0);
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
