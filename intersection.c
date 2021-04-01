/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 13:58:52 by acusanno          #+#    #+#             */
/*   Updated: 2021/04/01 14:07:20 by acusanno         ###   ########lyon.fr   */
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

int	intersection(t_vars *vars, t_lines line, t_point *res)
{
	double	slope_a;
	double	slope_b;
	t_pixel	a1;

	res->z = 0;
	a1 = vars->tp;
	slope_a = line_slope(a1.x, a1.y, a1.pdx + a1.x, a1.pdy + a1.y);
	slope_b = line_slope(line.a.x, line.a.y, line.b.x, line.b.y);
	if (slope_a == slope_b || (isnan(slope_a) && isnan(slope_b)))
	{
		res->z = -1;
		return (-1);
	}
	else if (isnan(slope_a) && !isnan(slope_b))
		set_x_y(res, a1.x, (a1.x - line.a.x) * slope_b + line.a.y);
	else if (isnan(slope_b) && !isnan(slope_a))
		set_x_y(res, line.a.x, (line.a.x - a1.x) * slope_a + a1.y);
	else
		set_x_y(res, (slope_a * a1.x - slope_b * line.a.x + line.a.y - a1.y) \
	    / (slope_a - slope_b), slope_b * (res->x - line.a.x) + line.a.y);
	return (1);
}

int	check_wall(t_vars *vars, float x, float y, char tal)
{
	int		i;
	int		j;

	i = 0;
	j = vars->tp.ri;
	if (tal == 'h' && vars->tp.ra < M_PI)
		i = 1;
	else if (tal == 'v' && vars->tp.ra > M_PI / 2
		&& vars->tp.ra < 3 * M_PI / 2)
		i = 1;
	if (x < 0 || y < 0 || x > vars->ts.map_width
		|| y > vars->ts.map_height)
	{
		if (tal == 'h')
			vars->tp.inter_h[j].z = -1;
		else
			vars->tp.inter_v[j].z = -1;
		return (-1);
	}
	if (tal == 'h' && vars->ts.map[(int)y - i][(int)x] == '1')
		return (1);
	if (tal == 'v' && vars->ts.map[(int)y][(int)x - i] == '1')
		return (1);
	return (0);
}

void	find_inter_h(t_vars *vars)
{
	int		i;
	int		ri;
	t_point	result;

	i = (int)vars->tp.y;
	ri = vars->tp.ri;
	while (i && i < vars->ts.map_height)
	{
		intersection(vars, vars->tg.h[i], &result);
		vars->tp.inter_h[ri] = result;
		if (check_wall(vars, vars->tp.inter_h[ri].x,
				vars->tp.inter_h[ri].y, 'h') != 0)
			break ;
		if (vars->tp.pa < M_PI)
			i++;
		if (vars->tp.pa > M_PI)
			i--;
	}
}

void	find_inter_v(t_vars *vars)
{
	int		i;
	int		ri;
	t_point	result;

	i = (int)vars->tp.x;
	ri = vars->tp.ri;
	if (vars->tp.ra > M_PI / 2 && vars->tp.ra < 3 * M_PI / 2)
		i++;
	while (i && i < vars->ts.map_width)
	{
		intersection(vars, vars->tg.v[i], &result);
		vars->tp.inter_v[ri] = result;
		if (check_wall(vars, vars->tp.inter_v[ri].x,
				vars->tp.inter_v[ri].y, 'v') != 0)
			break ;
		if (vars->tp.ra < M_PI / 2 || vars->tp.ra > 3 * M_PI / 2)
			i--;
		if (vars->tp.ra > M_PI / 2 && vars->tp.ra < 3 * M_PI / 2)
			i++;
	}
}
