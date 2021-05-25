/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 09:09:35 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/25 14:08:03 by acusanno         ###   ########lyon.fr   */
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

void	character_check(t_vars *vars, char *str)
{
	if (*str)
	{
		if (*str == 'R')
			parse_r(vars, str);
		else if (*str == 'N' || *str == 'S' || *str == 'W' || *str == 'E')
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

void	parse_r(t_vars *vars, char *str)
{
	int		i;
	char	**array;

	if (vars->ts.r[0] != 0 || vars->ts.r[1] != 0)
	{
		printf("Error\n Resolution defined multiple times\n");
		ft_exit(-1, str);
	}
	array = ft_split(str, ' ');
	i = ft_strlen_split(array);
	if (i != 3 || ft_strlen(array[0]) > 1)
	{
		printf("Error\n Wrong number of resolution arguments\n");
		ft_exit(-1, str);
	}
	vars->ts.r[0] = ft_atoi(array[1]);
	vars->ts.r[1] = ft_atoi(array[2]);
	free_tab(array);
	if (vars->ts.r[0] <= 0 || vars->ts.r[1] <= 0)
	{
		printf("Error\n Wrong resolution arguments\n");
		ft_exit(-1, str);
	}
}
