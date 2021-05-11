/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 13:58:52 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/11 11:31:31 by acusanno         ###   ########lyon.fr   */
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

// int	intersection(t_vars *vars, t_lines line, t_point *res, t_point a2)
// {
// 	double	slope_a;
// 	double	slope_b;
// 	t_pixel	a1;

// 	res->z = 0;
// 	a1 = vars->tp;
// 	slope_a = line_slope(a1.x, a1.y, a2.x, a2.y);
// 	slope_b = line_slope(line.a.x, line.a.y, line.b.x, line.b.y);
// 	if (slope_a == slope_b || (isnan(slope_a) && isnan(slope_b)))
// 	{
// 		res->z = -1;
// 		return (-1);
// 	}
// 	else if (isnan(slope_a) && !isnan(slope_b))
// 		set_x_y(res, a1.x, (a1.x - line.a.x) * slope_b + line.a.y);
// 	else if (isnan(slope_b) && !isnan(slope_a))
// 		set_x_y(res, line.a.x, (line.a.x - a1.x) * slope_a + a1.y);
// 	else
// 		set_x_y(res, (slope_a * a1.x - slope_b * line.a.x + line.a.y - a1.y)
// 			    / (slope_a - slope_b), slope_b * (res->x - line.a.x)
// 			+ line.a.y);
// 	return (1);
// }

t_point	intersection(t_point a1, t_point a2, t_point b1, t_point b2)
{
	double	slope_a;
	double	slope_b;
	t_point	res;

	res.z = 1;
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

t_point	rot_90(t_point sov)
{
	t_point	res;

	res.x = sov.x * -0.00000367 - sov.y;
	res.y = sov.x + sov.y * -0.00000367;
	res.z = sov.z;
	return (res);
}

t_point	get_vec(t_point first, t_point second)
{
	t_point		vec;

	vec.x = second.x - first.x;
	vec.y = second.y - first.y;
	return (vec);
}

void	find_inter_s(t_vars *vars, int sprite)
{
	int		ri;
	t_point	a2;
	t_point	b1;
	t_point	b2;
	t_point result;
	t_point	joueur;

	ri = vars->tp.ri;
	joueur.x = vars->tp.x;
	joueur.y = vars->tp.y;
	a2.x = joueur.x + cos(vars->tp.ra);
	a2.y = joueur.y - sin(vars->tp.ra);
	b1 = vars->ts.sprite[sprite];
	b2.x = b1.x + cos(vars->tp.pa + 89.5);
	b2.y = b1.y + -sin(vars->tp.pa + 89.5);
	if (vars->tp.inter_s[sprite][ri].z != -1)
	{
		result = intersection(joueur, a2, b1, b2);
		vars->tp.inter_s[sprite][ri].x = result.x;
		vars->tp.inter_s[sprite][ri].y = result.y;
		vars->tp.inter_s[sprite][ri].z = result.z;
	}
}

int	check_wall(t_vars *vars, float x, float y, char tal)
{
	int		i;
	int		j;

	i = 0;
	j = vars->tp.ri;
	if ((tal == 'h' && vars->tp.ra < M_PI)
		|| (tal == 'v' && vars->tp.ra > M_PI / 2
			&& vars->tp.ra < 3 * M_PI / 2))
		i = 1;
	if (x < 0 || y < 0 || x >= vars->ts.map_width
		|| y >= vars->ts.map_height)
	{
		if (tal == 'h')
			vars->tp.inter_h[j].z = -1;
		else
			vars->tp.inter_v[j].z = -1;
		return (-1);
	}
	if (tal == 'h' && y - 1 > 0 && x > 0
		&& (vars->ts.map[(int)y - i][(int)x] == '1'
		|| vars->ts.map[(int)y - i][(int)x] == '3'))
		return (1);
	else if (tal == 'h' && y - 1 > 0 && x > 0
		&& vars->ts.map[(int)y - i][(int)x] == '2')
		return (2);
	if (tal == 'v' && x - 1 > 0 && y > 0
		&& (vars->ts.map[(int)y][(int)x - i] == '1'
		|| vars->ts.map[(int)y][(int)x - i] == '3'))
		return (1);
	else if (tal == 'v' && x - 1 > 0 && y > 0
		&& vars->ts.map[(int)y][(int)x - i] == '2')
		return (2);
	return (0);
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

// int		check_wall_and_sprite(t_vars *vars, int ri, char face)
// {
// 	int	check;

// 	if (face == 'h')
// 		check = check_wall(vars, vars->tp.inter_h[ri].x,
// 				vars->tp.inter_h[ri].y, face);
// 	else
// 		check = check_wall(vars, vars->tp.inter_v[ri].x,
// 				vars->tp.inter_v[ri].y, face);
// 	if (check == 2 && face == 'h')
// 	{
// 		vars->tp.inter_s[sprite][ri].z = 1;
// 		vars->ts.sprite[0].x = (int)vars->tp.inter_h[ri].x + 0.5;
// 		if (vars->tp.ra > M_PI && vars->tp.ra < 2 * M_PI)
// 			vars->ts.sprite[0].y = (int)vars->tp.inter_h[ri].y + 0.5;
// 		else
// 			vars->ts.sprite[0].y = (int)vars->tp.inter_h[ri].y - 0.5;
// 	}
// 	else if (check == 2 && face == 'v')
// 	{
// 		vars->tp.inter_s[sprite][ri].z = 1;
// 		vars->ts.sprite[0].y = (int)vars->tp.inter_v[ri].y + 0.5;
// 		if (vars->tp.ra > M_PI / 2 && vars->tp.ra < 3 * M_PI / 2)
// 			vars->ts.sprite[0].x = (int)vars->tp.inter_v[ri].x - 0.5;
// 		else
// 			vars->ts.sprite[0].x = (int)vars->tp.inter_v[ri].x + 0.5;
// 	}
// 	if (check == 1 || check == 3)
// 		return (1);
// 	return (0);
// }

int		check_wall_h(t_vars *vars, int ri)
{
	int	check;
	int	sprite;

	check = wall_h(vars, vars->tp.inter_h[ri].x,
			vars->tp.inter_h[ri].y);
	if (check == 2)
	{
		sprite = which_sprite(vars, (int)vars->tp.inter_h[ri].x + 0.5,
				(int)vars->tp.inter_h[ri].y - 0.5);
		if (vars->tp.ra > M_PI && vars->tp.ra < 2 * M_PI)
			sprite = which_sprite(vars, (int)vars->tp.inter_h[ri].x + 0.5,
					(int)vars->tp.inter_h[ri].y + 0.5);
		vars->tp.inter_s[sprite][ri].z = 1;
	}
	if (check == 1 || check == 3)
		return (1);
	else if (check == -1)
		return (-1);
	return (0);
}

int		check_wall_v(t_vars *vars, int ri)
{
	int	check;
	int	sprite;

	check = wall_v(vars, vars->tp.inter_v[ri].x,
			vars->tp.inter_v[ri].y);
	if (check == 2)
	{
		sprite = which_sprite(vars, (int)vars->tp.inter_v[ri].x + 0.5,
				(int)vars->tp.inter_v[ri].y + 0.5);
		if (vars->tp.ra > M_PI / 2 && vars->tp.ra < 3 * M_PI / 2)
			sprite = which_sprite(vars, (int)vars->tp.inter_v[ri].x - 0.5,
					(int)vars->tp.inter_v[ri].y + 0.5);
		vars->tp.inter_s[sprite][ri].z = 1;
	}
	if (check == 1 || check == 3)
		return (1);
	else if (check == -1)
		return (-1);
	return (0);
}

int		which_sprite(t_vars *vars, float x, float y)
{
	int	i;

	i = 0;
	while (i < vars->ts.nb_sp)
	{
		if (vars->ts.sprite[i].x == x & vars->ts.sprite[i].y == y)
			return (i);
		i++;
	}
	return (-1);
}

void	find_inter_h(t_vars *vars)
{
	int		i;
	int		ri;
	t_point	a2;
	t_point	joueur;
	t_point	result;

	i = (int)vars->tp.y;
	ri = vars->tp.ri;
	joueur.x = vars->tp.x;
	joueur.y = vars->tp.y;
	a2.x = vars->tp.x + cos(vars->tp.ra);
	a2.y = vars->tp.y - sin(vars->tp.ra);
	if (vars->tp.ra > M_PI && vars->tp.ra < 2 * M_PI)
		i++;
	while (i && i < vars->ts.map_height)
	{
		result = intersection(joueur, a2, vars->tg.h[i].a, vars->tg.h[i].b);
		vars->tp.inter_h[ri] = result;
		if (check_wall_h(vars, ri) == 1)
			break ;
		if (vars->tp.ra < M_PI && vars->tp.ra > 0)
			i--;
		else if (vars->tp.ra > M_PI && vars->tp.ra < 2 * M_PI)
			i++;
	}
}

void	find_inter_v(t_vars *vars)
{
	int		i;
	int		ri;
	t_point	a2;
	t_point	joueur;
	t_point	result;

	i = (int)vars->tp.x;
	ri = vars->tp.ri;
	joueur.x = vars->tp.x;
	joueur.y = vars->tp.y;
	a2.x = vars->tp.x + cos(vars->tp.ra);
	a2.y = vars->tp.y - sin(vars->tp.ra);
	if (vars->tp.ra < M_PI / 2 || vars->tp.ra > 3 * M_PI / 2)
		i++;
	while (i && i < vars->ts.map_width)
	{
		result = intersection(joueur, a2, vars->tg.v[i].a, vars->tg.v[i].b);
		vars->tp.inter_v[ri] = result;
		if (check_wall_v(vars, ri) == 1)
			break ;
		if (vars->tp.ra < M_PI / 2 || vars->tp.ra > 3 * M_PI / 2)
			i++;
		else if (vars->tp.ra > M_PI / 2 && vars->tp.ra < 3 * M_PI / 2)
			i--;
	}
}
