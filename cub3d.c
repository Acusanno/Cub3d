/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:14:42 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/25 08:05:33 by acusanno         ###   ########lyon.fr   */
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
	float	ratioangle;
	int		i;

	ratio_height = 0;
	vars->tp.ri = vars->ts.r[0];
	ratioangle = (M_PI / 3) / vars->ts.r[0];
	vars->tp.ra = vars->tp.pa - M_PI / 6;
	while (vars->tp.ri >= 0)
	{
		rotation(vars);
		i = texture_index(vars->tp.ra, vars->tp.face[vars->tp.ri]);
		draw_column(vars, vars->tp.ri, i);
		draw_all_sprite(vars);
		vars->tp.ra += ratioangle;
		vars->tp.ri--;
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

void	find_all_sprite(t_vars *vars)
{
	int	i;

	i = -1;
	while (++i < vars->ts.nb_sp)
	{
		rotation(vars);
		if (vars->tp.vis_sp[i])
			find_inter_s(vars, i);
		if (vars->tp.dist[vars->tp.ri] < dist(vars->tp.player,
				vars->tp.inter_s[i][vars->tp.ri]))
			vars->tp.inter_s[i][vars->tp.ri].z = -1;
	}
}

void	find_all_inter(t_vars *vars)
{
	float	ratioangle;

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
		find_all_sprite(vars);
		vars->tp.ra += ratioangle;
		vars->tp.ri--;
	}
}
