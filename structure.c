/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 11:14:28 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/19 11:15:12 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	inter_malloc(t_vars *vars)
{
	int	i;

	i = 0;
	vars->tp.inter_h = malloc(sizeof(t_point) * (vars->ts.r[0] + 1));
	vars->tp.inter_v = malloc(sizeof(t_point) * (vars->ts.r[0] + 1));
	vars->tp.inter_s = malloc(sizeof(t_point *) * (vars->ts.nb_sp + 1));
	vars->tp.dist_sp = malloc(sizeof(float *) * (vars->ts.nb_sp + 1));
	while (i < vars->ts.nb_sp)
	{
		vars->tp.inter_s[i] = malloc(sizeof(t_point) * (vars->ts.r[0] + 1));
		vars->tp.dist_sp[i] = malloc(sizeof(float) * (vars->ts.r[0] + 1));
		i++;
	}
	vars->tp.face = malloc(sizeof(char) * (vars->ts.r[0] + 1));
	vars->tp.dist = malloc(sizeof(int) * (vars->ts.r[0] + 2));
	vars->tp.vis_sp = malloc(sizeof(int) * vars->ts.nb_sp + 1);
}

void	inter_init(t_vars *vars, int ri)
{
	int	i;

	i = 0;
	vars->tp.inter_h[ri].x = 0;
	vars->tp.inter_v[ri].x = 0;
	vars->tp.inter_h[ri].y = 0;
	vars->tp.inter_v[ri].y = 0;
	vars->tp.inter_h[ri].z = 0;
	vars->tp.inter_v[ri].z = 0;
	while (i < vars->ts.nb_sp)
	{
		vars->tp.inter_s[i][ri].x = 0;
		vars->tp.inter_s[i][ri].y = 0;
		vars->tp.inter_s[i][ri].z = 0;
		if (vars->tp.vis_sp[i])
			vars->tp.vis_sp[i] = 0;
		i++;
	}
}
