/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 11:16:09 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/19 13:53:29 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	move_forward(t_vars *vars, int shift)
{
	if (vars->ts.map[(int)(vars->tp.player.y + vars->tp.pdy / 4)]
		[(int)vars->tp.player.x] != '1' && vars->ts.map[(int)
		(vars->tp.player.y + vars->tp.pdy / 4)][(int)vars->tp.player.x] != '2')
	{
		if (shift == 1)
			vars->tp.player.y += vars->tp.pdy / 8;
		else
			vars->tp.player.y += vars->tp.pdy / 15;
	}
	if (vars->ts.map[(int)vars->tp.player.y]
		[(int)(vars->tp.player.x + vars->tp.pdx / 4)]
		!= '1' && vars->ts.map[(int)vars->tp.player.y]
		[(int)(vars->tp.player.x + vars->tp.pdx / 4)] != '2')
	{
		if (shift == 1)
			vars->tp.player.x += vars->tp.pdx / 8;
		else
			vars->tp.player.x += vars->tp.pdx / 15;
	}
}

void	move_backward(t_vars *vars)
{
	if (vars->ts.map[(int)(vars->tp.player.y - vars->tp.pdy / 4)]
		[(int)vars->tp.player.x] != '1' && vars->ts.map[(int)(vars->tp.player.y
		- vars->tp.pdy / 4)][(int)vars->tp.player.x] != '2')
		vars->tp.player.y -= vars->tp.pdy / 15;
	if (vars->ts.map[(int)vars->tp.player.y][(int)(vars->tp.player.x
		- vars->tp.pdx / 4)] != '1' && vars->ts.map[(int)vars->tp.player.y]
		[(int)(vars->tp.player.x - vars->tp.pdx / 4)] != '2')
		vars->tp.player.x -= vars->tp.pdx / 15;
}

void	move_leftward(t_vars *vars)
{
	if (vars->ts.map[(int)vars->tp.player.y]
		[(int)(vars->tp.player.x + cos(vars->tp.pa + (M_PI / 2)) / 4)] != '1'
		&& vars->ts.map[(int)vars->tp.player.y][(int)(vars->tp.player.x
		+ cos(vars->tp.pa + (M_PI / 2)) / 4)] != '2')
		vars->tp.player.x += cos(vars->tp.pa + (M_PI / 2)) / 30;
	if (vars->ts.map[(int)(vars->tp.player.y - sin(vars->tp.pa
				+ (M_PI / 2)) / 4)]
		[(int)vars->tp.player.x] != '1' && vars->ts.map[(int)(vars->tp.player.y
		- sin(vars->tp.pa + (M_PI / 2)) / 4)][(int)vars->tp.player.x] != '2')
		vars->tp.player.y -= sin(vars->tp.pa + (M_PI / 2)) / 30;
}

void	move_rightward(t_vars *vars)
{
	if (vars->ts.map[(int)vars->tp.player.y][(int)(vars->tp.player.x
		+ cos(vars->tp.pa - (M_PI / 2)) / 4)] != '1'
		&& vars->ts.map[(int)vars->tp.player.y][(int)(vars->tp.player.x
		+ cos(vars->tp.pa - (M_PI / 2)) / 4)] != '2')
		vars->tp.player.x += cos(vars->tp.pa - (M_PI / 2)) / 30;
	if (vars->ts.map[(int)(vars->tp.player.y - sin(vars->tp.pa
				- (M_PI / 2)) / 4)][(int)vars->tp.player.x] != '1'
		&& vars->ts.map[(int)(vars->tp.player.y - sin(vars->tp.pa - (M_PI / 2))
		/ 4)][(int)vars->tp.player.x] != '2')
		vars->tp.player.y -= sin(vars->tp.pa - (M_PI / 2)) / 30;
}

void	update_player_pos(t_vars *vars)
{
	vars->tp.pdx = cos(vars->tp.pa);
	vars->tp.pdy = -sin(vars->tp.pa);
	if (vars->tc.left == 1)
		vars->tp.pa += M_PI / 35;
	if (vars->tc.right == 1)
		vars->tp.pa -= M_PI / 35;
	rotation(vars);
	if (vars->tc.w == 1)
		move_forward(vars, vars->tc.shift);
	if (vars->tc.s)
		move_backward(vars);
	if (vars->tc.a == 1)
		move_leftward(vars);
	if (vars->tc.d == 1)
		move_rightward(vars);
	if (vars->tp.pa == 0 || vars->tp.pa == 2 * M_PI || vars->tp.pa == M_PI / 2
		|| vars->tp.pa == 3 * M_PI / 2)
		vars->tp.pa += 0.1;
}
