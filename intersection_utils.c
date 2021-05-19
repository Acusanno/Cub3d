/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 11:09:44 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/19 11:12:19 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	line_slope(float a1, float a2, float b1, float b2)
{
	if (a1 - b1 == 0.0)
		return (NAN);
	else
		return ((a2 - b2) / (a1 - b1));
}

int	wall_h(t_vars *vars, float x, float y)
{
	int		i;
	int		j;

	i = 0;
	j = vars->tp.ri;
	if (vars->tp.ra < M_PI)
		i = 1;
	if (x < 0 || y < 0 || x >= vars->ts.map_width
		|| y >= vars->ts.map_height)
	{
		vars->tp.inter_h[j].z = -1;
		return (-1);
	}
	if (y - 1 > 0 && x > 0 && (vars->ts.map[(int)y - i][(int)x] == '1'
		|| vars->ts.map[(int)y - i][(int)x] == '3'))
		return (1);
	else if (y - 1 > 0 && x > 0 && vars->ts.map[(int)y - i][(int)x] == '2')
		return (2);
	return (0);
}

int	wall_v(t_vars *vars, float x, float y)
{
	int		i;
	int		j;

	i = 0;
	j = vars->tp.ri;
	if (vars->tp.ra > M_PI / 2 && vars->tp.ra < 3 * M_PI / 2)
		i = 1;
	if (x < 0 || y < 0 || x >= vars->ts.map_width
		|| y >= vars->ts.map_height)
	{
		vars->tp.inter_v[j].z = -1;
		return (-1);
	}
	if (x - 1 > 0 && y > 0 && (vars->ts.map[(int)y][(int)x - i] == '1'
		|| vars->ts.map[(int)y][(int)x - i] == '3'))
		return (1);
	else if (x - 1 > 0 && y > 0 && vars->ts.map[(int)y][(int)x - i] == '2')
		return (2);
	return (0);
}

int		which_sprite(t_vars *vars, float x, float y)
{
	int	i;

	i = 0;
	while (i < vars->ts.nb_sp)
	{
		if (vars->ts.sprite[i].x == x + 0.5 && vars->ts.sprite[i].y == y + 0.5)
			return (i);
		i++;
	}
	return (-1);
}

t_point	intersection(t_point a1, t_point a2, t_point b1, t_point b2)
{
	double	slope_a;
	double	slope_b;
	t_point	res;

	res.z = 0;
	slope_a = line_slope(a1.x, a1.y, a2.x, a2.y);
	slope_b = line_slope(b1.x, b1.y, b2.x, b2.y);
	if (isnan(slope_a) && !isnan(slope_b))
	{
		res.x = a1.x;
		res.y = (a1.x - b1.x) * slope_b + b1.y;
	}
	else if (isnan(slope_b) && !isnan(slope_a))
	{
		res.x = b1.x;
		res.y = (b1.x - a1.x) * slope_a + a1.y;
	}
	else
	{
		res.x = (slope_a * a1.x - slope_b * b1.x + b1.y - a1.y) \
		/ (slope_a - slope_b);
		res.y = slope_b * (res.x - b1.x) + b1.y;
	}
	return (res);
}
