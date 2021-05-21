/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:14:42 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/21 10:25:02 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

float	dist(t_point a, t_point b)
{
	float	res;

	res = sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));
	return (res);
}

void	draw_screen(t_vars *vars)
{
	float	ratio_height;
	int		ri;
	float	ratioangle;
	int		i;

	ratio_height = 0;
	ri = vars->ts.r[0];
	ratioangle = (M_PI / 3) / vars->ts.r[0];
	vars->tp.ra = vars->tp.pa - M_PI / 6;
	while (ri >= 0)
	{
		rotation(vars);
		ratio_height = vars->ts.r[1] / vars->tp.dist[ri];
		i = texture_index(vars->tp.ra, vars->tp.face[ri]);
		draw_column(vars, ri, ratio_height, i);
		i = 0;
		while (i < vars->ts.nb_sp)
		{
			if (vars->tp.vis_sp[i] && vars->tp.dist_sp[i][ri] > 0.5)
			{
				ratio_height = vars->ts.r[1] / vars->tp.dist_sp[i][ri];
				draw_sprite(vars, ri, ratio_height, i);
			}
			i++;
		}
		vars->tp.ra += ratioangle;
		ri--;
	}
}

int	render_next_frame(t_vars *vars)
{
	update_player_pos(vars);
	dist_center_sprite(vars);
	sprite_ordering(vars);
	find_all_inter(vars);
	draw_screen(vars);
	if (vars->tc.tab == 1)
	{
		map_draw(vars);
		player_draw(vars);
		rays_draw(vars);
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	mlx_do_sync(vars->mlx);
	return (0);
}

void	find_all_inter(t_vars *vars)
{
	float	ratioangle;
	int		i;

	vars->tp.ri = vars->ts.r[0];
	ratioangle = (M_PI / 3) / vars->ts.r[0];
	vars->tp.ra = vars->tp.pa - M_PI / 6;
	while (vars->tp.ri >= 0)
	{
		inter_init(vars, vars->tp.ri);
		vars->tp.ri--;
	}
	vars->tp.ri = vars->ts.r[0];
	while (vars->tp.ri >= 0)
	{
		rotation(vars);
		find_inter_h(vars);
		find_inter_v(vars);
		rotation(vars);
		distance_comp(vars, vars->tp.ri);
		vars->tp.dist[vars->tp.ri] *= cos(vars->tp.pa - vars->tp.ra);
		i = 0;
		while (i < vars->ts.nb_sp)
		{
			rotation(vars);
			if (vars->tp.vis_sp[i])
				find_inter_s(vars, i);
			if (vars->tp.dist[vars->tp.ri] < dist(vars->tp.player,
					vars->tp.inter_s[i][vars->tp.ri]))
				vars->tp.inter_s[i][vars->tp.ri].z = -1;
			i++;
		}
		vars->tp.ra += ratioangle;
		vars->tp.ri--;
	}
}
