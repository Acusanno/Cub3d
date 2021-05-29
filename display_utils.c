/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 10:24:34 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/29 11:37:23 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	player_draw(t_vars *vars)
{
	vars->tp.pdx = cos(vars->tp.pa) * 5;
	vars->tp.pdy = -sin(vars->tp.pa) * 5;
	my_mlx_pixel_put(vars, vars->tp.player.x * vars->minimap_size,
		vars->tp.player.y * vars->minimap_size, 0x00FF0000);
	my_mlx_pixel_put(vars,
		vars->tp.player.x * vars->minimap_size + vars->tp.pdx,
		vars->tp.player.y * vars->minimap_size + vars->tp.pdy, 0x00FF00FF);
}

void	rays_draw(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->ts.r[0])
	{
		my_mlx_pixel_put(vars, vars->tp.inter_h[i].x
			* vars->minimap_size, vars->tp.inter_h[i].y
			* vars->minimap_size, 0x00FF00FF);
		i++;
	}
}
