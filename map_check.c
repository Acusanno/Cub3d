/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 09:11:08 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/15 13:54:57 by acusanno         ###   ########lyon.fr   */
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

int		is_spawn(char c)
{
	if (c == 'N' || c == 'S' || c == 'O' || c == 'E')
		return (1);
	return (0);
}

void	error_map(void)
{
	printf("Error\n Map invalid");
	exit(-1);
}

void	check_resolution(t_vars *vars)
{
	int x;
	int y;

	x = 0;
	y = 0;
	mlx_get_screen_size(vars->mlx, &x, &y);
	if (vars->ts.r[0] > x)
		vars->ts.r[0] = x;
	if (vars->ts.r[1] > y)
		vars->ts.r[1] = y;
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
	if (!(vars->ts.r[0]) || !(vars->ts.r[1]))
	{
		printf("Error\n Wrong number of settings defined\n");
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
	int		spawn;

	i = 0;
	j = 0;
	spawn = 0;
	while (ts->map[i])
	{
		j = 0;
		while (ts->map[i][j])
		{
			if (ts->map[i][j] != '1' && ts->map[i][j] != ' ')
			{
				if (is_spawn(ts->map[i][j]) == 1)
				{
					spawn++;
					ts->spawn = ts->map[i][j];
				}
				check_sides(ts, i, j);
			}
			j++;
		}
		i++;
	}
	if (spawn != 1)
		error_map();
}
