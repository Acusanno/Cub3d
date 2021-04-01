/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:14:42 by acusanno          #+#    #+#             */
/*   Updated: 2021/04/01 14:31:49 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char		*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	key_hook(int keycode, t_vars *vars)
{
	if (keycode == UP)
	{
		if (vars->run == 0)
		{
			vars->tp.y += vars->tp.pdy / 10;
			vars->tp.x += vars->tp.pdx / 10;
		}
		else
		{
			vars->tp.y += vars->tp.pdy / 5;
			vars->tp.x += vars->tp.pdx / 5;
		}
	}
	if (keycode == DOWN)
	{
		if (vars->run == 0)
		{
			vars->tp.y -= vars->tp.pdy / 10;
			vars->tp.x -= vars->tp.pdx / 10;
		}
		else
		{
			vars->tp.y -= vars->tp.pdy / 5;
			vars->tp.x -= vars->tp.pdx / 5;
		}
	}
	if (keycode == LEFT)
		vars->tp.pa += M_PI / 20;
	if (keycode == RIGHT)
		vars->tp.pa -= M_PI / 20;
	if (keycode == ESC)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
	if (keycode == TAB && vars->minimap_display == 0)
		vars->minimap_display = 1;
	else if (keycode == TAB && vars->minimap_display == 1)
		vars->minimap_display = 0;
	if (keycode == SHIFT && vars->run == 0)
		vars->run = 1;
	else if (keycode == SHIFT && vars->run == 1)
		vars->run = 0;
	if (vars->tp.pa < 0)
		vars->tp.pa += 2 * M_PI;
	else if (vars->tp.pa > (2 * M_PI))
		vars->tp.pa -= 2 * M_PI;
	vars->tp.pdx = cos(vars->tp.pa) * 5;
	vars->tp.pdy = -sin(vars->tp.pa) * 5;
	find_all_inter(vars);
	printf("Keycode = %d\n", keycode);
	return (0);
}

int	shutdown(int keycode, t_vars *vars)
{
	(void)keycode;
	(void)vars;
	exit(0);
	return (0);
}

int	render_next_frame(t_vars *vars)
{
	if (vars->minimap_display == 1)
	{
		map_draw(vars);
		my_mlx_pixel_put(&vars->img, vars->tp.x * vars->minimap_size,
			vars->tp.y * vars->minimap_size, 0x00FF0000);
		my_mlx_pixel_put(&vars->img,
			vars->tp.x * vars->minimap_size + vars->tp.pdx,
			vars->tp.y * vars->minimap_size + vars->tp.pdy, 0x00FF00FF);
		// if (vars->tp.inter_h[30].z != -1 && vars->tp.inter_h[30].x < vars->ts.map_width
		// 	&& vars->tp.inter_h[30].x > 0)
		// 	my_mlx_pixel_put(&vars->img, vars->tp.inter_h[30].x * vars->minimap_size,
		// 		vars->tp.inter_h[30].y * vars->minimap_size, 0x00FF00FF);
		// if (vars->tp.inter_v[30].z != -1 && vars->tp.inter_v[30].x < vars->ts.map_width
		// 	&& vars->tp.inter_v[30].x > 0 && vars->tp.inter_v[30].y >= 0)
		// 	my_mlx_pixel_put(&vars->img, vars->tp.inter_v[30].x * vars->minimap_size,
		// 		vars->tp.inter_v[30].y * vars->minimap_size, 0x00FF00FF);
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	return (0);
}

void	find_all_inter(t_vars *vars)
{
	float	ratioangle;

	vars->tp.inter_h = malloc(sizeof(t_point) * (vars->ts.r[1] + 1));
	vars->tp.inter_v = malloc(sizeof(t_point) * (vars->ts.r[0] + 1));
	vars->tp.ri = 0;
	ratioangle = M_PI / 3 / vars->ts.r[1];
	vars->tp.ra = vars->tp.pa - M_PI / 6;
	while (vars->tp.ri < vars->ts.r[1])
	{
		find_inter_h(vars);
		find_inter_v(vars);
		vars->tp.ra += ratioangle;
		vars->tp.ri++;
	}
}

int		main(int argc, char **argv)
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
		vars.minimap_display = 1;
		vars.run = 0;
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
		printf("\033[92mColor ciel\033[0m --> [%d]\n", vars.ts.c);
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
	vars.tp.pdx = cos(vars.tp.pa) * 5;
	vars.tp.pdy = -sin(vars.tp.pa) * 5;
	//key hook, met le bouton pressé a 1
	mlx_hook(vars.win, 2, 1L << 0, key_hook, &vars);
	//key hook (release), le bouton est mis a 0 au relachement
	// mlx_hook(vars.win, 2, 1L << 0, key_hook, &vars);
	mlx_hook(vars.win, 17, 0L, &shutdown, &vars);
	vars.img.img = mlx_new_image(vars.mlx, vars.ts.r[0], vars.ts.r[1]);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel,
			&vars.img.line_length, &vars.img.endian);
	mlx_loop_hook(vars.mlx, render_next_frame, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
