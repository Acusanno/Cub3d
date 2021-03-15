/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 17:16:35 by acusanno          #+#    #+#             */
/*   Updated: 2020/11/26 11:18:37 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	nb_character(long int n)
{
	long int i;

	i = 0;
	if (n <= 0)
	{
		n = -n;
		i++;
	}
	while (n > 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char		*ft_itoa(int n)
{
	char		*str;
	int			j;
	long int	tot;

	tot = n;
	j = nb_character(n) - 1;
	if (!(str = malloc(sizeof(char) * (nb_character(tot) + 1))))
		return (0);
	if (tot < 0)
	{
		str[0] = '-';
		tot = -tot;
	}
	else if (tot == 0)
		str[0] = '0';
	while (tot > 0)
	{
		str[j--] = tot % 10 + '0';
		tot /= 10;
	}
	str[nb_character(n)] = '\0';
	return (str);
}
