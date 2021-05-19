/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 13:20:54 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/19 13:21:17 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	my_mlx_pixel_put(t_vars *vars, int x, int y, int color)
{
	char		*dst;

	if (x >= 0 && y >= 0 && x < vars->ts.r[0] && y < vars->ts.r[1])
	{
		dst = vars->img.addr + (y * vars->img.line_length
				+ x * (vars->img.bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

int	shutdown(int keycode, t_vars *vars)
{
	(void)keycode;
	(void)vars;
	ft_exit(0, vars, NULL);
	return (0);
}

void	ft_exit(int code, t_vars *vars, char *truc)
{
	(void)vars;
	if (truc != NULL)
		free(truc);
	exit(code);
}
