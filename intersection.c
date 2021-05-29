/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 13:58:52 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/29 11:39:09 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_wall_h(t_vars *vars, int ri)
{
	int	check;

	check = wall_h(vars, vars->tp.inter_h[ri].x,
			vars->tp.inter_h[ri].y);
	if (check == 1)
		return (1);
	else if (check == -1)
		return (-1);
	return (0);
}

int	check_wall_v(t_vars *vars, int ri)
{
	int	check;

	check = wall_v(vars, vars->tp.inter_v[ri].x,
			vars->tp.inter_v[ri].y);
	if (check == 1)
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
