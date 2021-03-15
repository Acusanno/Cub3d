/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_size.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 10:26:14 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/15 14:01:54 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	map_size(t_settings *ts)
{
	int i;

	i = 0;
	ts->map_width = 0;
	ts->map_height = 0;
	while (ts->map[ts->map_height])
	{
		i = ft_strlen(ts->map[ts->map_height]);
		if (i > ts->map_width)
			ts->map_width = i;
		ts->map_height++;
	}
}

void	spawn_player(t_vars *vars)
{
	int i;
	int imap;
	int j;
	int jmap;

	j = 0;
	while (j < vars->ts.map_height)
	{
		i = 0;
		while (i < vars->ts.map_width)
		{
			imap = i * vars->minimap_size;
			jmap = j * vars->minimap_size;
			if (i >= (int)ft_strlen(vars->ts.map[j]))
				break ;
			else if (is_spawn(vars->ts.map[j][i]) == 1)
			{
				vars->tp.x = imap;
				vars->tp.y = jmap;
				i++;
			}
			i++;
		}
		j++;
	}
}

void	draw_cube(int x, int y, int color, t_vars *vars)
{
	int i;
	int j;

	i = 0;
	while (i < vars->minimap_size)
	{
		j = 0;
		while (j < vars->minimap_size)
		{
			my_mlx_pixel_put(&vars->img, x + i, y + j, color);
			j++;
		}
		i++;
	}
}

void	map_draw(t_vars *vars)
{
	int i;
	int imap;
	int j;
	int jmap;

	j = 0;
	while (j < vars->ts.map_height)
	{
		i = 0;
		while (i < vars->ts.map_width)
		{
			imap = i * vars->minimap_size;
			jmap = j * vars->minimap_size;
			if (i >= (int)ft_strlen(vars->ts.map[j]))
				draw_cube(imap, jmap, 0x0000ff, vars);
			else if (vars->ts.map[j][i] == ' ')
				draw_cube(imap, jmap, 0x0000ff, vars);
			else if (vars->ts.map[j][i] == '1')
				draw_cube(imap, jmap, 0xffffff, vars);
			else if (vars->ts.map[j][i] == '0')
				draw_cube(imap, jmap, 0x00ffff, vars);
			else if (is_spawn(vars->ts.map[j][i]) == 1)
				draw_cube(imap, jmap, 0x00ffff, vars);
			i++;
		}
		j++;
	}
}
