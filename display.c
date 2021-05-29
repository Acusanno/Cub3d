/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 11:24:46 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/29 11:37:16 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	text_x_wall(t_vars *vars, int j, int ri)
{
	if (vars->tp.face[ri] == 'h')
		vars->td[j].text_x = fmodf(vars->tp.inter_h[ri].x, 1)
			* vars->td[j].img_width;
	else if (vars->tp.face[ri] == 'v')
		vars->td[j].text_x = fmodf(vars->tp.inter_h[ri].y, 1)
			* vars->td[j].img_width;
}

void	start_end_init(t_vars *vars, float ratio_height)
{
	vars->tp.start = (vars->ts.r[1] - ratio_height) / 2;
	vars->tp.end = (vars->ts.r[1] + ratio_height) / 2;
	if (vars->tc.ctrl == 1)
		vars->tp.start -= vars->ts.r[1] / 10;
	if (vars->tc.ctrl == 1)
		vars->tp.end -= vars->ts.r[1] / 10;
}

void	draw_column(t_vars *vars, int ri, int j)
{
	int		i;
	int		color;
	float	ratio_height;

	ratio_height = vars->ts.r[1] / vars->tp.dist[vars->tp.ri];
	start_end_init(vars, ratio_height);
	i = -1;
	while (++i < vars->tp.start)
		my_mlx_pixel_put(vars, ri, i, vars->ts.c);
	text_x_wall(vars, j, ri);
	while (i < vars->tp.end)
	{
		vars->td[j].text_y = (i - vars->tp.start) * vars->td[j].img_height
			/ (ratio_height);
		if (i < vars->ts.r[1] && ri < vars->ts.r[0] && i >= 0 && ri >= 0)
		{
			color = vars->td[j].iaddr[vars->td[j].text_y * vars->td[j].img_width
				+ vars->td[j].text_x];
			my_mlx_pixel_put(vars, ri, i, color);
		}
		i++;
	}
	i--;
	while (i++ < vars->ts.r[1])
		my_mlx_pixel_put(vars, ri, i, vars->ts.f);
}
