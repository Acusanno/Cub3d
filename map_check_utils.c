/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 08:50:13 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/25 10:20:21 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_space(t_settings *ts, int i, int j)
{
	if (!(ts->map[i][j]) || ts->map[i][j] == ' ')
	{
		printf("Error\n Map invalid");
		exit(-1);
	}
}

int	is_spawn(char c)
{
	if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
		return (1);
	return (0);
}

void	error_map(void)
{
	printf("Error\n Map invalid");
	exit(-1);
}

void	put_spawn(t_settings *ts, int i, size_t j)
{
	if (is_spawn(ts->map[i][j]) == 1)
		ts->nb_spawn++;
	if (is_spawn(ts->map[i][j]) == 1)
		ts->spawn = ts->map[i][j];
	else if (ts->map[i][j] == '2')
		ts->nb_sp++;
	else if ((ts->map[i][j] != '3' && ts->map[i][j] != '0'
		&& is_spawn(ts->map[i][j]) != 1))
	{
		printf("Error\n Character in map doesn't exist");
		ft_exit(-1, NULL, NULL);
	}
}
