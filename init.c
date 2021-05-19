/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 13:54:35 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/19 11:17:45 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	lines_init(t_vars *vars)
{
	int	i;

	vars->tg.h = ft_calloc(vars->ts.map_height + 1, sizeof(t_lines));
	vars->tg.v = ft_calloc(vars->ts.map_width + 1, sizeof(t_lines));
	i = 0;
	while (i < vars->ts.map_height)
	{
		vars->tg.h[i].a.x = 0;
		vars->tg.h[i].a.y = i;
		vars->tg.h[i].b.x = 1;
		vars->tg.h[i].b.y = i;
		i++;
	}
	i = 0;
	while (i < vars->ts.map_width)
	{
		vars->tg.v[i].a.x = i;
		vars->tg.v[i].a.y = 0;
		vars->tg.v[i].b.x = i;
		vars->tg.v[i].b.y = 1;
		i++;
	}
}

void	spawn_init(t_vars *vars)
{
	if (vars->ts.spawn == 'N')
		vars->tp.pa = (M_PI / 2);
	if (vars->ts.spawn == 'S')
		vars->tp.pa = ((3 * M_PI) / 2);
	if (vars->ts.spawn == 'E')
		vars->tp.pa = 0;
	if (vars->ts.spawn == 'W')
		vars->tp.pa = M_PI;
}

void	set_x_y(t_point *res, double first, double second)
{
	res->x = first;
	res->y = second;
}

void	controls_init(t_vars *vars)
{
	vars->tc.a = 0;
	vars->tc.d = 0;
	vars->tc.w = 0;
	vars->tc.s = 0;
	vars->tc.left = 0;
	vars->tc.right = 0;
	vars->tc.shift = 0;
	vars->tc.tab = 0;
	vars->tc.ctrl = 0;
}

void	map_transform(t_vars *vars)
{
	int		i;
	int		j;
	char	**newmap;

	i = 0;
	newmap = malloc(sizeof(char *) * vars->ts.map_height + 1);
	while (vars->ts.map[i])
	{
		newmap[i] = malloc(sizeof(char) * vars->ts.map_width + 1);
		j = 0;
		while (j < vars->ts.map_width)
		{
			if (j < (int)ft_strlen(vars->ts.map[i]))
				newmap[i][j] = vars->ts.map[i][j];
			else
				newmap[i][j] = ' ';
			j++;
		}
		newmap[i][j] = '\0';
		i++;
	}
	free_tab(vars->ts.map);
	vars->ts.map = newmap;
}
