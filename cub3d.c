/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:14:42 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/19 14:41:35 by acusanno         ###   ########lyon.fr   */
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
	int	i;

	update_player_pos(vars);
	dist_center_sprite(vars);
	sprite_ordering(vars);
	vars->tp.pdx = cos(vars->tp.pa) * 5;
	vars->tp.pdy = -sin(vars->tp.pa) * 5;
	find_all_inter(vars);
	draw_screen(vars);
	if (vars->tc.tab == 1)
	{
		map_draw(vars);
		my_mlx_pixel_put(vars, vars->tp.player.x * vars->minimap_size,
			vars->tp.player.y * vars->minimap_size, 0x00FF0000);
		my_mlx_pixel_put(vars,
			vars->tp.player.x * vars->minimap_size + vars->tp.pdx,
			vars->tp.player.y * vars->minimap_size + vars->tp.pdy, 0x00FF00FF);
		i = 0;
		while (i < vars->ts.r[0])
		{
			if (vars->tp.inter_h[i].x <= vars->ts.map_width
				&& vars->tp.inter_h[i].y <= vars->ts.map_height)
				my_mlx_pixel_put(vars, vars->tp.inter_h[i].x
					* vars->minimap_size, vars->tp.inter_h[i].y
					* vars->minimap_size, 0x00FF00FF);
			i++;
		}
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


int	main(int argc, char **argv)
{
	int		i;
	int		j;
	t_vars	vars;

	i = 0;
	j = 0;
	struct_init(&vars.ts);
	if (argc == 2)
	{
		vars.ts.map = NULL;
		vars.ts.filename = argv[1];
		vars.mlx = mlx_init();
		parse_settings(&vars);
		settings_check(&vars);
		map_check(&vars.ts);
		map_size(&vars.ts);
		map_transform(&vars);
		sprite_init(&vars);
		if (vars.ts.map_width > vars.ts.map_height)
			vars.minimap_size = vars.ts.r[0] / (vars.ts.map_width * 3);
		else
			vars.minimap_size = vars.ts.r[1] / (vars.ts.map_height * 3);
		printf("\n\n\033[33m----------INFOS---------------\033[0m\n");
		printf("\033[92mRes Horizontale\033[0m --> [%d]\n", vars.ts.r[0]);
		printf("\033[92mRes Verticale\033[0m --> [%d]\n", vars.ts.r[1]);
		printf("\033[92mNO\033[0m --> [%s]\n", vars.ts.no);
		printf("\033[92mSO\033[0m --> [%s]\n", vars.ts.so);
		printf("\033[92mWE\033[0m --> [%s]\n", vars.ts.we);
		printf("\033[92mEA\033[0m --> [%s]\n", vars.ts.ea);
		printf("\033[92mS\033[0m --> [%s]\n", vars.ts.s);
		printf("\033[92mColor floor\033[0m --> [%d]\n", vars.ts.f);
		printf("\033[92mColor sky\033[0m --> [%d]\n", vars.ts.c);
		printf("\033[33m-----------MAP---------------\033[0m\n");
		while (i < vars.ts.map_height)
		{
			printf("\033[92mligne [%2d] :\033[0m %s", i, vars.ts.map[i]);
			i++;
			printf("\n");
		}
	}
	else
		printf("Error\nWrong number of arguments.\n");
	vars.win = mlx_new_window(vars.mlx, vars.ts.r[0], vars.ts.r[1], "Cub3d");
	spawn_player(&vars);
	spawn_init(&vars);
	lines_init(&vars);
	inter_malloc(&vars);
	// vars.tp.pdx = cos(vars.tp.pa);
	// vars.tp.pdy = -sin(vars.tp.pa);
	controls_init(&vars);
	mlx_hook(vars.win, 2, 1L << 0, key_pressed, &vars);
	mlx_hook(vars.win, 3, 1L << 1, key_released, &vars);
	mlx_hook(vars.win, 17, 0L, &shutdown, &vars);
	vars.img.img = mlx_new_image(vars.mlx, vars.ts.r[0], vars.ts.r[1]);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel,
			&vars.img.line_length, &vars.img.endian);
	vars.img.iaddr = (int *)mlx_get_data_addr(vars.img.img,
			&vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);
	read_all_img(&vars);
	mlx_loop_hook(vars.mlx, render_next_frame, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
