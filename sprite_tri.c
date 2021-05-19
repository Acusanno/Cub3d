/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_tri.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 13:00:18 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/19 13:23:06 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	sprite_init(t_vars *vars)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
	vars->ts.dist_center_sp = malloc(sizeof(float) * vars->ts.nb_sp);
	vars->ts.sprite = malloc(sizeof(t_point) * vars->ts.nb_sp);
	while (i < vars->ts.map_height)
	{
		j = 0;
		while (j < vars->ts.map_width)
		{
			if (vars->ts.map[i][j] == '2')
			{
				vars->ts.sprite[k].x = j + 0.5;
				vars->ts.sprite[k].y = i + 0.5;
				k++;
			}
			j++;
		}
		i++;
	}
}

void	dist_center_sprite(t_vars *vars)
{
	int		i;

	i = 0;
	while (i < vars->ts.nb_sp)
	{
		vars->ts.dist_center_sp[i] = dist(vars->tp.player, vars->ts.sprite[i]);
		i++;
	}
}

void	sprite_ordering(t_vars *vars)
{
	int		i;
	float	swap_dist;
	t_point	swap_sprite;
	int		swap_vis;

	i = 0;
	while (i < vars->ts.nb_sp - 1)
	{
		if (vars->ts.dist_center_sp[i] < vars->ts.dist_center_sp[i + 1])
		{
			swap_dist = vars->ts.dist_center_sp[i + 1];
			vars->ts.dist_center_sp[i + 1] = vars->ts.dist_center_sp[i];
			vars->ts.dist_center_sp[i] = swap_dist;
			swap_sprite = vars->ts.sprite[i + 1];
			vars->ts.sprite[i + 1] = vars->ts.sprite[i];
			vars->ts.sprite[i] = swap_sprite;
			swap_vis = vars->tp.vis_sp[i + 1];
			vars->tp.vis_sp[i + 1] = vars->tp.vis_sp[i];
			vars->tp.vis_sp[i] = swap_vis;
			i = -1;
		}
		i++;
	}
}
