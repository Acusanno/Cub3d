/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 13:58:52 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/25 08:40:37 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	find_inter_s(t_vars *vars, int sprite)
{
	t_point	a2;
	t_point	b1;
	t_point	b2;
	t_point	result;

	a2.x = vars->tp.player.x + cos(vars->tp.ra);
	a2.y = vars->tp.player.y - sin(vars->tp.ra);
	b1 = vars->ts.sprite[sprite];
	b2.x = b1.x + cos(vars->tp.pa + M_PI / 2);
	b2.y = b1.y - sin(vars->tp.pa + M_PI / 2);
	if (vars->tp.inter_s[sprite][vars->tp.ri].z != -1)
	{
		result = intersection(vars->tp.player, a2, b1, b2);
		vars->tp.inter_s[sprite][vars->tp.ri].x = result.x;
		vars->tp.inter_s[sprite][vars->tp.ri].y = result.y;
		vars->tp.inter_s[sprite][vars->tp.ri].z = result.z;
		vars->tp.dist_sp[sprite][vars->tp.ri] = dist(vars->tp.player,
				vars->tp.inter_s[sprite][vars->tp.ri]);
		vars->tp.dist_sp[sprite][vars->tp.ri]
			*= cos(vars->tp.pa - vars->tp.ra) / 1.3;
	}
}

int	check_wall_h(t_vars *vars, int ri)
{
	int	check;
	int	sprite;

	check = wall_h(vars, vars->tp.inter_h[ri].x,
			vars->tp.inter_h[ri].y);
	if (check == 2)
	{
		sprite = which_sprite(vars, (int)vars->tp.inter_h[ri].x,
				(int)vars->tp.inter_h[ri].y - 1);
		if (vars->tp.ra > M_PI && vars->tp.ra < 2 * M_PI)
			sprite = which_sprite(vars, (int)vars->tp.inter_h[ri].x,
					(int)vars->tp.inter_h[ri].y);
		vars->tp.vis_sp[sprite] = 1;
	}
	else if (check == 1 || check == 3)
		return (1);
	else if (check == -1)
		return (-1);
	return (0);
}

int	check_wall_v(t_vars *vars, int ri)
{
	int	check;
	int	sprite;

	check = wall_v(vars, vars->tp.inter_v[ri].x,
			vars->tp.inter_v[ri].y);
	if (check == 2)
	{
		sprite = which_sprite(vars, (int)vars->tp.inter_v[ri].x,
				(int)vars->tp.inter_v[ri].y);
		if (vars->tp.ra > M_PI / 2 && vars->tp.ra < 3 * M_PI / 2)
			sprite = which_sprite(vars, (int)vars->tp.inter_v[ri].x - 1,
					(int)vars->tp.inter_v[ri].y);
		vars->tp.vis_sp[sprite] = 1;
	}
	else if (check == 1 || check == 3)
		return (1);
	else if (check == -1)
		return (-1);
	return (0);
}

void	find_inter_h(t_vars *vars)
{
	int		i;
	int		ri;
	t_point	a2;
	t_point	result;

	i = (int)vars->tp.player.y;
	ri = vars->tp.ri;
	a2.x = vars->tp.player.x + cos(vars->tp.ra);
	a2.y = vars->tp.player.y - sin(vars->tp.ra);
	if (vars->tp.ra > M_PI && vars->tp.ra < 2 * M_PI)
		i++;
	while (i && i < vars->ts.map_height)
	{
		result = intersection(vars->tp.player, a2, vars->tg.h[i].a,
				vars->tg.h[i].b);
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
	t_point	result;

	i = (int)vars->tp.player.x;
	ri = vars->tp.ri;
	a2.x = vars->tp.player.x + cos(vars->tp.ra);
	a2.y = vars->tp.player.y - sin(vars->tp.ra);
	if (vars->tp.ra < M_PI / 2 || vars->tp.ra > 3 * M_PI / 2)
		i++;
	while (i && i < vars->ts.map_width)
	{
		result = intersection(vars->tp.player, a2, vars->tg.v[i].a,
				vars->tg.v[i].b);
		vars->tp.inter_v[ri] = result;
		if (check_wall_v(vars, ri) == 1)
			break ;
		if (vars->tp.ra < M_PI / 2 || vars->tp.ra > 3 * M_PI / 2)
			i++;
		else if (vars->tp.ra > M_PI / 2 && vars->tp.ra < 3 * M_PI / 2)
			i--;
	}
}
