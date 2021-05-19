/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 12:33:29 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/19 12:35:03 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	read_img(t_vars *vars, int i)
{
	vars->td[i].img = mlx_xpm_file_to_image(vars->mlx, vars->td[i].path,
			&vars->td[i].img_width, &vars->td[i].img_height);
	vars->td[i].iaddr = (int *)mlx_get_data_addr(vars->td[i].img,
			&vars->td[i].bits_per_pixel, &vars->td[i].line_length,
			&vars->td[i].endian);
}

void	read_all_img(t_vars *vars)
{
	vars->td = calloc(5, sizeof(t_data));
	put_path(vars);
	read_img(vars, 0);
	read_img(vars, 1);
	read_img(vars, 2);
	read_img(vars, 3);
	read_img(vars, 4);
}

int	texture_index(float angle, char face)
{
	if (face == 'h' && angle > 0 && angle < M_PI)
		return (0);
	else if (face == 'h')
		return (1);
	if (face == 'v' && (angle < M_PI / 2 || angle > 3 * M_PI / 2))
		return (3);
	else
		return (2);
}

void	put_path(t_vars *vars)
{
	vars->td[0].path = vars->ts.no;
	vars->td[1].path = vars->ts.so;
	vars->td[2].path = vars->ts.we;
	vars->td[3].path = vars->ts.ea;
	vars->td[4].path = vars->ts.s;
}
