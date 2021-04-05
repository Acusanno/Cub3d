/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:14:42 by acusanno          #+#    #+#             */
/*   Updated: 2021/04/05 13:40:57 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char		*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	shutdown(int keycode, t_vars *vars)
{
	(void)keycode;
	(void)vars;
	exit(0);
	return (0);
}

void	update_player_pos(t_vars *vars)
{
	vars->tp.pdx = cos(vars->tp.pa) * 5;
	vars->tp.pdy = -sin(vars->tp.pa) * 5;
	if (vars->tc.w == 1 && vars->tc.shift == 1)
	{
		if (vars->ts.map[(int)(vars->tp.y + vars->tp.pdy / 30)][(int)vars->tp.x] != '1')
			vars->tp.y += vars->tp.pdy / 30;
		if (vars->ts.map[(int)vars->tp.y][(int)(vars->tp.x + vars->tp.pdx / 30)] != '1')
			vars->tp.x += vars->tp.pdx / 30;
	}
	else if (vars->tc.w == 1)
	{
		if (vars->ts.map[(int)(vars->tp.y + vars->tp.pdy / 80)][(int)vars->tp.x] != '1')
			vars->tp.y += vars->tp.pdy / 80;
		if (vars->ts.map[(int)vars->tp.y][(int)(vars->tp.x + vars->tp.pdx / 80)] != '1')
			vars->tp.x += vars->tp.pdx / 80;
	}
	if (vars->tc.s)
	{
		if (vars->ts.map[(int)(vars->tp.y - vars->tp.pdy / 80)][(int)vars->tp.x] != '1')
			vars->tp.y -= vars->tp.pdy / 80;
		if (vars->ts.map[(int)vars->tp.y][(int)(vars->tp.x - vars->tp.pdx / 80)] != '1')
			vars->tp.x -= vars->tp.pdx / 80;
	}
	if (vars->tc.left == 1)
		vars->tp.pa += M_PI / 35;
	if (vars->tc.right == 1)
		vars->tp.pa -= M_PI / 35;
	if (vars->tc.a == 1)
	{
		if (vars->ts.map[(int)vars->tp.y][(int)(vars->tp.x + cos(vars->tp.pa + (M_PI / 2)) / 30)] != '1')
			vars->tp.x += cos(vars->tp.pa + (M_PI / 2)) / 30;
		if (vars->ts.map[(int)(vars->tp.y - sin(vars->tp.pa + (M_PI / 2)) / 30)][(int)vars->tp.x] != '1')
			vars->tp.y -= sin(vars->tp.pa + (M_PI / 2)) / 30;
	}
	if (vars->tc.d == 1)
	{
		if (vars->ts.map[(int)vars->tp.y][(int)(vars->tp.x + cos(vars->tp.pa - (M_PI / 2)) / 30)] != '1')
			vars->tp.x += cos(vars->tp.pa - (M_PI / 2)) / 30;
		if (vars->ts.map[(int)(vars->tp.y - sin(vars->tp.pa - (M_PI / 2)) / 30)][(int)vars->tp.x] != '1')
			vars->tp.y -= sin(vars->tp.pa - (M_PI / 2)) / 30;
	}
	if (vars->tp.pa < 0)
		vars->tp.pa += 2 * M_PI;
	else if (vars->tp.pa > (2 * M_PI))
		vars->tp.pa -= 2 * M_PI;
}

int	render_next_frame(t_vars *vars)
{
	int	i;

	update_player_pos(vars);
	vars->tp.pdx = cos(vars->tp.pa) * 5;
	vars->tp.pdy = -sin(vars->tp.pa) * 5;
	find_all_inter(vars);
	if (vars->tc.tab == 0)
	{
		map_draw(vars);
		my_mlx_pixel_put(&vars->img, vars->tp.x * vars->minimap_size,
			vars->tp.y * vars->minimap_size, 0x00FF0000);
		my_mlx_pixel_put(&vars->img,
			vars->tp.x * vars->minimap_size + vars->tp.pdx,
			vars->tp.y * vars->minimap_size + vars->tp.pdy, 0x00FF00FF);
		i = 0;
		while (i < vars->ts.r[0])
		{
			if (vars->tp.inter_h[i].x <= vars->ts.map_width
				&& vars->tp.inter_h[i].y <= vars->ts.map_height
				&& vars->tp.inter_h[i].x >= 0
				&& vars->tp.inter_h[i].y >= 0)
				my_mlx_pixel_put(&vars->img, vars->tp.inter_h[i].x
					* vars->minimap_size, vars->tp.inter_h[i].y
					* vars->minimap_size, 0x00FF00FF);
			i++;
		}
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	return (0);
}

t_point	find_closest(t_vars *vars, t_point inter_h, t_point inter_v)
{
	t_point	result;

	if (inter_h.z == -1 && inter_v.z != -1)
		return (inter_v);
	if (inter_v.z == -1 && inter_h.z != -1)
		return (inter_h);
	if (sqrt(pow(vars->tp.x - inter_h.x, 2) + pow(vars->tp.y - inter_h.y, 2))
		< sqrt(pow(vars->tp.x - inter_v.x, 2)
			+ pow(vars->tp.y - inter_v.y, 2)))
	{
		result.x = inter_h.x;
		result.y = inter_h.y;
	}
	else
	{
		result.x = inter_v.x;
		result.y = inter_v.y;
	}
	return (result);
}

void	find_all_inter(t_vars *vars)
{
	float	ratioangle;

	vars->tp.inter_h = malloc(sizeof(t_point) * (vars->ts.r[0] + 1));
	vars->tp.inter_v = malloc(sizeof(t_point) * (vars->ts.r[0] + 1));
	vars->tp.ri = 0;
	ratioangle = (M_PI / 3) / vars->ts.r[0];
	vars->tp.ra = vars->tp.pa - M_PI / 6;
	while (vars->tp.ri < vars->ts.r[0])
	{
		vars->tp.inter_h[vars->tp.ri].x = 0;
		vars->tp.inter_v[vars->tp.ri].x = 0;
		vars->tp.inter_h[vars->tp.ri].y = 0;
		vars->tp.inter_v[vars->tp.ri].y = 0;
		vars->tp.inter_h[vars->tp.ri].z = 0;
		vars->tp.inter_v[vars->tp.ri].z = 0;
		vars->tp.ri++;
	}
	vars->tp.ri = 0;
	while (vars->tp.ri < vars->ts.r[0])
	{
		if (vars->tp.ra < 0)
			vars->tp.ra += 2 * M_PI;
		else if (vars->tp.ra > (2 * M_PI))
			vars->tp.ra -= 2 * M_PI;
		find_inter_h(vars);
		find_inter_v(vars);
		// if (vars->tp.inter_h[vars->tp.ri].z == -1)
		// {
		// 	vars->tp.inter_h[vars->tp.ri].x = 0;
		// 	vars->tp.inter_h[vars->tp.ri].y = 0;
		// }
		// if (vars->tp.inter_v[vars->tp.ri].z == -1)
		// {
		// 	vars->tp.inter_v[vars->tp.ri].x = 0;
		// 	vars->tp.inter_v[vars->tp.ri].y = 0;
		// }
		vars->tp.inter_h[vars->tp.ri] = find_closest(vars, vars->tp.inter_h[vars->tp.ri],
				vars->tp.inter_v[vars->tp.ri]);
		vars->tp.ra += ratioangle;
		vars->tp.ri++;
	}
	dprintf(1, "h[0].x = %f, h[0].y = %f\n", vars->tp.inter_h[0].x, vars->tp.inter_h[0].y);
	// dprintf(1, "v[0].z = %f\n", vars->tp.inter_v[0].z);
	// dprintf(1, "h[0].z = %f\n", vars->tp.inter_h[0].z);
	dprintf(1, "h[500].x = %f, h[500].y = %f\n", vars->tp.inter_h[500].x, vars->tp.inter_h[500].y);
	// dprintf(1, "v[500].z = %f\n", vars->tp.inter_v[500].z);
	// dprintf(1, "h[500].z = %f\n", vars->tp.inter_h[500].z);
	dprintf(1, "h[999].x = %f, h[999].y = %f\n", vars->tp.inter_h[999].x, vars->tp.inter_h[999].y);
	// dprintf(1, "v[999].z = %f\n", vars->tp.inter_v[999].z);
	// dprintf(1, "h[999].z = %f\n", vars->tp.inter_h[999].z);
}

int	key_pressed(int keycode, t_vars *vars)
{
	if (keycode == UP || keycode == W)
		vars->tc.w = 1;
	if (keycode == LEFT)
		vars->tc.left = 1;
	if (keycode == RIGHT)
		vars->tc.right = 1;
	if (keycode == A)
		vars->tc.a = 1;
	if (keycode == D)
		vars->tc.d = 1;
	if (keycode == DOWN || keycode == S)
		vars->tc.s = 1;
	if (keycode == TAB)
		vars->tc.tab = 1;
	if (keycode == SHIFT)
		vars->tc.shift = 1;
	if (keycode == ESC)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
	return (0);
}

int	key_released(int keycode, t_vars *vars)
{
	if (keycode == UP || keycode == W)
		vars->tc.w = 0;
	if (keycode == LEFT)
		vars->tc.left = 0;
	if (keycode == RIGHT)
		vars->tc.right = 0;
	if (keycode == A)
		vars->tc.a = 0;
	if (keycode == D)
		vars->tc.d = 0;
	if (keycode == DOWN || keycode == S)
		vars->tc.s = 0;
	if (keycode == TAB)
		vars->tc.tab = 0;
	if (keycode == SHIFT)
		vars->tc.shift = 0;
	return (0);
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
		vars.tv.r = 1;
		vars.mlx = mlx_init();
		parse_settings(&vars.ts);
		settings_check(&vars);
		map_check(&vars.ts);
		map_size(&vars.ts);
		map_transform(&vars);
		if (vars.ts.map_width > vars.ts.map_height)
			vars.minimap_size = vars.ts.r[0] / vars.ts.map_width;
		else
			vars.minimap_size = vars.ts.r[1] / vars.ts.map_height;
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
			printf("\033[92mligne [%d] :\033[0m %s", i, vars.ts.map[i]);
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
	vars.tp.pdx = cos(vars.tp.pa);
	vars.tp.pdy = -sin(vars.tp.pa);
	mlx_hook(vars.win, 2, 1L << 0, key_pressed, &vars);
	mlx_hook(vars.win, 3, 1L << 1, key_released, &vars);
	mlx_hook(vars.win, 17, 0L, &shutdown, &vars);
	vars.img.img = mlx_new_image(vars.mlx, vars.ts.r[0], vars.ts.r[1]);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel,
			&vars.img.line_length, &vars.img.endian);
	mlx_loop_hook(vars.mlx, render_next_frame, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
