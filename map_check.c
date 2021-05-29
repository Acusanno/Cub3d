/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 09:11:08 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/29 11:47:21 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_resolution(t_vars *vars)
{
	vars->ts.r[0] = 1920;
	vars->ts.r[1] = 1080;
}

void	settings_check(t_vars *vars)
{
	if (!(vars->ts.no) || !(vars->ts.so || !(vars->ts.we)))
	{
		printf("Error\n Wrong number of settings defined\n");
		exit(-1);
	}
	if (!(vars->ts.ea) || (vars->ts.f == -1) || (vars->ts.c == -1))
	{
		printf("Error\n Wrong number of settings defined\n");
		exit(-1);
	}
	if (!(vars->ts.map))
	{
		printf("Error\n There is no map\n");
		exit(-1);
	}
	check_resolution(vars);
}

void	check_sides(t_settings *ts, int i, size_t j)
{
	if (i != 0)
		check_space(ts, i - 1, j);
	else
		error_map();
	if (j != 0)
		check_space(ts, i, j - 1);
	else
		error_map();
	if (j != ft_strlen(ts->map[i]) - 1)
		check_space(ts, i, j + 1);
	else
		error_map();
	if (i != ft_strlen_split(ts->map) - 1)
		check_space(ts, i + 1, j);
	else
		error_map();
}

void	map_check(t_settings *ts)
{
	int		i;
	size_t	j;

	i = 0;
	ts->nb_spawn = 0;
	ts->nb_sp = 0;
	while (ts->map[i])
	{
		j = 0;
		while (ts->map[i][j])
		{
			if (ts->map[i][j] != '1' && ts->map[i][j] != ' ')
			{
				put_spawn(ts, i, j);
				check_sides(ts, i, j);
			}
			j++;
		}
		i++;
	}
	if (ts->nb_spawn != 1)
		error_map();
}
