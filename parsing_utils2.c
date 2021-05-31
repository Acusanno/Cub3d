/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 09:09:35 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/31 11:14:46 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	ft_strlen_split(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

int	comma_count(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[j])
	{
		if (str[j] == ',')
			i++;
		j++;
	}
	return (i);
}

void	character_check(t_vars *vars, char *str)
{
	if (*str)
	{
		if (*str == 'N' || *str == 'S' || *str == 'W' || *str == 'E')
			parse_nswes(vars, str, 0);
		else if (*str == 'F' || *str == 'C')
			parse_fc(vars, str);
		else if (*str == ' ' || *str == '1')
		{
			parse_map(vars, str);
			free(str);
			return ;
		}
		else
		{
			printf("Error\n Settings invalid");
			ft_exit(-1, str);
		}
	}
	free(str);
}
