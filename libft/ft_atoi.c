/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 13:00:05 by acusanno          #+#    #+#             */
/*   Updated: 2020/12/02 14:53:14 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	int i;
	int out;
	int sign;
	int len;

	len = 0;
	i = 0;
	sign = 1;
	out = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		sign = -sign;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		out = out * 10 + (str[i++] - 48);
		len++;
	}
	if (len > 18 && sign == 1)
		return (-1);
	else if (len > 18 && sign == -1)
		return (0);
	return (sign * out);
}
