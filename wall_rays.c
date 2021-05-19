/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 13:13:51 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/19 13:14:58 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_point	close_wall(t_vars *vars, t_point inter_h, t_point inter_v)
{
	t_point	result;

	if (inter_h.z == -1)
	{
		vars->tp.face[vars->tp.ri] = 'v';
		return (inter_v);
	}
	if (inter_v.z == -1)
	{
		vars->tp.face[vars->tp.ri] = 'h';
		return (inter_h);
	}
	if (dist(vars->tp.player, inter_h) < dist(vars->tp.player, inter_v))
	{
		result.x = inter_h.x;
		result.y = inter_h.y;
		vars->tp.face[vars->tp.ri] = 'h';
	}
	else
	{
		result.x = inter_v.x;
		result.y = inter_v.y;
		vars->tp.face[vars->tp.ri] = 'v';
	}
	return (result);
}

void	distance_comp(t_vars *vars, int ri)
{
	vars->tp.inter_h[ri] = close_wall(vars, vars->tp.inter_h[ri],
			vars->tp.inter_v[ri]);
	if (vars->tp.face[vars->tp.ri] == 'h')
		vars->tp.dist[ri] = dist(vars->tp.player, vars->tp.inter_h[ri]);
	else
		vars->tp.dist[ri] = dist(vars->tp.player, vars->tp.inter_v[ri]);
}

void	rotation(t_vars *vars)
{
	if (vars->tp.ra < 0)
		vars->tp.ra += 2 * M_PI;
	else if (vars->tp.ra > (2 * M_PI))
		vars->tp.ra -= 2 * M_PI;
	else if (vars->tp.pa < 0)
		vars->tp.pa += 2 * M_PI;
	else if (vars->tp.pa > (2 * M_PI))
		vars->tp.pa -= 2 * M_PI;
}
