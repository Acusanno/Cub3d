/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:14:42 by acusanno          #+#    #+#             */
/*   Updated: 2021/04/12 14:38:58 by acusanno         ###   ########lyon.fr   */
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

void	free_struct(t_vars *vars)
{
	free(vars->ts.r);
	if (vars->ts.s != NULL)
		free(vars->ts.s);
	if (vars->ts.no != NULL)
		free(vars->ts.no);
	if (vars->ts.so != NULL)
		free(vars->ts.so);
	if (vars->ts.we != NULL)
		free(vars->ts.we);
	if (vars->ts.ea != NULL)
		free(vars->ts.ea);
	if (vars->ts.map != NULL)
		free(vars->ts.map);
	if (vars->tp.inter_h != NULL)
		free(vars->tp.inter_h);
	if (vars->tp.inter_v != NULL)
		free(vars->tp.inter_v);
	if (vars->tp.face != NULL)
		free(vars->tp.face);
	if (vars->tp.dist != NULL)
		free(vars->tp.dist);
}

void	ft_exit(int code, t_vars *vars, char *truc)
{
	//free_struct(vars);
	(void)vars;
	if (truc != NULL)
		free(truc);
	exit(code);
}

void	update_player_pos(t_vars *vars)
{
	vars->tp.pdx = cos(vars->tp.pa) * 5;
	vars->tp.pdy = -sin(vars->tp.pa) * 5;
	if (vars->tc.w == 1 && vars->tc.shift == 1)
	{
		if (vars->ts.map[(int)(vars->tp.y + vars->tp.pdy / 30)]
			[(int)vars->tp.x] != '1')
			vars->tp.y += vars->tp.pdy / 30;
		if (vars->ts.map[(int)vars->tp.y]
			[(int)(vars->tp.x + vars->tp.pdx / 30)] != '1')
			vars->tp.x += vars->tp.pdx / 30;
	}
	else if (vars->tc.w == 1)
	{
		if (vars->ts.map[(int)(vars->tp.y + vars->tp.pdy / 80)]
			[(int)vars->tp.x] != '1')
			vars->tp.y += vars->tp.pdy / 80;
		if (vars->ts.map[(int)vars->tp.y]
			[(int)(vars->tp.x + vars->tp.pdx / 80)] != '1')
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

void	draw_column(t_vars *vars, int ri, float ratio_height)
{
	int	start;
	int	end;
	int	i;

	i = 0;
	start = (vars->ts.r[1] - ratio_height) / 2;
	end = (vars->ts.r[1] + ratio_height) / 2;
	if (vars->tc.ctrl == 1)
		start -= 80;
	if (vars->tc.ctrl == 1)
		end -= 80;
	while (i < start && i < vars->ts.r[1])
	{
		my_mlx_pixel_put(vars, ri, i, 0x51F4F5);
		i++;
	}
	while (i < end && i < vars->ts.r[1])
	{
		if (vars->tp.face[vars->tp.ri - ri] == 'v')
			my_mlx_pixel_put(vars, ri, i, 0x3345FF);
		else
			my_mlx_pixel_put(vars, ri, i, 0x6672E8);
		i++;
	}
	while (i < vars->ts.r[1])
	{
		my_mlx_pixel_put(vars, ri, i, 0x52280F);
		i++;
	}
}

void	draw_screen(t_vars *vars)
{
	float	ratio_height;
	int		ri;

	ratio_height = 0;
	ri = vars->tp.ri;
	while (ri > 0)
	{
		ratio_height = vars->ts.r[1] / vars->tp.dist[ri];
		draw_column(vars, vars->tp.ri - ri, ratio_height);
		ri--;
	}
}

int	render_next_frame(t_vars *vars)
{
	int	i;

	update_player_pos(vars);
	vars->tp.pdx = cos(vars->tp.pa) * 5;
	vars->tp.pdy = -sin(vars->tp.pa) * 5;
	find_all_inter(vars);
	if (vars->tc.tab == 0)
		draw_screen(vars);
	if (vars->tc.tab == 1)
	{
		map_draw(vars);
		my_mlx_pixel_put(vars, vars->tp.x * vars->minimap_size,
			vars->tp.y * vars->minimap_size, 0x00FF0000);
		my_mlx_pixel_put(vars,
			vars->tp.x * vars->minimap_size + vars->tp.pdx,
			vars->tp.y * vars->minimap_size + vars->tp.pdy, 0x00FF00FF);
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
	// mlx_put_image_to_window(vars->mlx, vars->win, vars->td[1].img, 0, 0);
	vars->imgswap = vars->img;
	vars->img = vars->img2;
	vars->img2 = vars->img;
	return (0);
}

t_point	find_closest(t_vars *vars, t_point inter_h, t_point inter_v)
{
	t_point	result;

	if (inter_h.z == -1 && inter_v.z != -1)
	{
		vars->tp.face[vars->tp.ri] = 'v';
		return (inter_v);
	}
	if (inter_v.z == -1 && inter_h.z != -1)
	{
		vars->tp.face[vars->tp.ri] = 'h';
		return (inter_h);
	}
	if (sqrt(pow(vars->tp.x - inter_h.x, 2) + pow(vars->tp.y - inter_h.y, 2))
		< sqrt(pow(vars->tp.x - inter_v.x, 2)
			+ pow(vars->tp.y - inter_v.y, 2)))
	{
		result.x = inter_h.x;
		result.y = inter_h.y;
		vars->tp.face[vars->tp.ri] = 'h';
	}
	else
	{
		result.x = inter_v.x;
		result.y = inter_v.y;
		vars->tp.face[vars->tp.ri] = 'v';
	}
	return (result);
}

void	inter_init(t_vars *vars, int ri)
{
	vars->tp.inter_h[ri].x = 0;
	vars->tp.inter_v[ri].x = 0;
	vars->tp.inter_h[ri].y = 0;
	vars->tp.inter_v[ri].y = 0;
	vars->tp.inter_h[ri].z = 0;
	vars->tp.inter_v[ri].z = 0;
}

void	distance_comp(t_vars *vars, int ri)
{
	find_inter_h(vars);
	find_inter_v(vars);
	vars->tp.inter_h[ri] = find_closest(vars, vars->tp.inter_h[ri],
			vars->tp.inter_v[ri]);
	if (sqrtf(pow(vars->tp.x - vars->tp.inter_h[ri].x, 2)
			+ powf(vars->tp.y - vars->tp.inter_h[ri].y, 2))
		< sqrtf(powf(vars->tp.x - vars->tp.inter_v[ri].x, 2)
			+ powf(vars->tp.y - vars->tp.inter_v[ri].y, 2)))
	{
		vars->tp.dist[ri] = sqrtf(powf(vars->tp.x - vars->tp.inter_h[ri].x, 2)
				+ powf(vars->tp.y - vars->tp.inter_h[ri].y, 2));
	}
	else
	{
		vars->tp.dist[ri] = sqrtf(powf(vars->tp.x - vars->tp.inter_v[ri].x, 2)
				+ powf(vars->tp.y - vars->tp.inter_v[ri].y, 2));
	}
}

void	find_all_inter(t_vars *vars)
{
	float	ratioangle;

	if (vars->tp.inter_h)
	{
		free(vars->tp.inter_h);
		free(vars->tp.inter_v);
		free(vars->tp.face);
		free(vars->tp.dist);
	}
	vars->tp.inter_h = malloc(sizeof(t_point) * (vars->ts.r[0] + 1));
	vars->tp.inter_v = malloc(sizeof(t_point) * (vars->ts.r[0] + 1));
	vars->tp.face = malloc(sizeof(char) * (vars->ts.r[0] + 1));
	vars->tp.dist = malloc(sizeof(int) * (vars->ts.r[0] + 2));
	vars->tp.ri = 0;
	ratioangle = (M_PI / 3) / vars->ts.r[0];
	vars->tp.ra = vars->tp.pa - M_PI / 6;
	while (vars->tp.ri <= vars->ts.r[0])
	{
		inter_init(vars, vars->tp.ri);
		vars->tp.ri++;
	}
	vars->tp.ri = 0;
	while (vars->tp.ri <= vars->ts.r[0])
	{
		if (vars->tp.ra < 0)
			vars->tp.ra += 2 * M_PI;
		else if (vars->tp.ra > (2 * M_PI))
			vars->tp.ra -= 2 * M_PI;
		distance_comp(vars, vars->tp.ri);
		vars->tp.dist[vars->tp.ri] *= cos(vars->tp.pa - vars->tp.ra);
		vars->tp.ra += ratioangle;
		vars->tp.ri++;
	}
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
	if (keycode == SHIFT && (vars->tc.w == 1))
		vars->tc.shift = 1;
	if (keycode == CTRL)
		vars->tc.ctrl = 1;
	if (keycode == ESC)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		ft_exit(0, vars, NULL);
	}
	return (0);
}

int	key_released(int keycode, t_vars *vars)
{
	if (keycode == UP || keycode == W)
		vars->tc.w = 0;
	if (keycode == UP || keycode == W)
		vars->tc.shift = 0;
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
	if (keycode == CTRL)
		vars->tc.ctrl = 0;
	return (0);
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

void	read_img(t_vars *vars, int i)
{
	vars->td[i].img = mlx_xpm_file_to_image(vars->mlx, vars->td[i].path,
			&vars->td[i].img_width, &vars->td[i].img_height);
	mlx_get_data_addr(vars->td[i].img, &vars->td[i].bits_per_pixel,
		&vars->td[i].line_length, &vars->td[i].endian);
}

void	put_path(t_vars *vars)
{
	vars->td[0].path = vars->ts.no;
	vars->td[1].path = vars->ts.so;
	vars->td[2].path = vars->ts.we;
	vars->td[3].path = vars->ts.ea;
	vars->td[4].path = vars->ts.s;
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
	controls_init(&vars);
	mlx_hook(vars.win, 2, 1L << 0, key_pressed, &vars);
	mlx_hook(vars.win, 3, 1L << 1, key_released, &vars);
	mlx_hook(vars.win, 17, 0L, &shutdown, &vars);
	vars.img.img = mlx_new_image(vars.mlx, vars.ts.r[0], vars.ts.r[1]);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel,
			&vars.img.line_length, &vars.img.endian);
	vars.img2.img = mlx_new_image(vars.mlx, vars.ts.r[0], vars.ts.r[1]);
	vars.img2.addr = mlx_get_data_addr(vars.img2.img, &vars.img2.bits_per_pixel,
			&vars.img2.line_length, &vars.img2.endian);
	read_all_img(&vars);
	mlx_loop_hook(vars.mlx, render_next_frame, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
