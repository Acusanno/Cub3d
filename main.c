/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 08:33:25 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/29 11:39:12 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	print_map(t_settings ts)
{
	int	i;

	i = 0;
	printf("\n\n\033[33m----------INFOS---------------\033[0m\n");
	printf("\033[92mRes Horizontale\033[0m --> [%d]\n", ts.r[0]);
	printf("\033[92mRes Verticale\033[0m --> [%d]\n", ts.r[1]);
	printf("\033[92mNO\033[0m --> [%s]\n", ts.no);
	printf("\033[92mSO\033[0m --> [%s]\n", ts.so);
	printf("\033[92mWE\033[0m --> [%s]\n", ts.we);
	printf("\033[92mEA\033[0m --> [%s]\n", ts.ea);
	printf("\033[92mS\033[0m --> [%s]\n", ts.s);
	printf("\033[92mColor floor\033[0m --> [%d]\n", ts.f);
	printf("\033[92mColor sky\033[0m --> [%d]\n", ts.c);
	printf("\033[33m-----------MAP---------------\033[0m\n");
	while (i < ts.map_height)
	{
		printf("\033[92mligne [%2d] :\033[0m %s", i, ts.map[i]);
		i++;
		printf("\n");
	}
}

void	parsing(t_vars *vars)
{
	vars->ts.map = NULL;
	vars->mlx = mlx_init();
	parse_settings(vars);
	settings_check(vars);
	map_check(&vars->ts);
	map_size(&vars->ts);
	map_transform(vars);
	if (vars->ts.map_width > vars->ts.map_height)
		vars->minimap_size = vars->ts.r[0] / (vars->ts.map_width * 3);
	else
		vars->minimap_size = vars->ts.r[1] / (vars->ts.map_height * 3);
	spawn_player(vars);
	spawn_init(vars);
	lines_init(vars);
	inter_malloc(vars);
	controls_init(vars);
	read_all_img(vars);
}	

void	create_img(t_vars *vars)
{
	vars->img.img = mlx_new_image(vars->mlx, vars->ts.r[0], vars->ts.r[1]);
	vars->img.addr = mlx_get_data_addr(vars->img.img,
			&vars->img.bits_per_pixel, &vars->img.line_length,
			&vars->img.endian);
	vars->img.iaddr = (int *)mlx_get_data_addr(vars->img.img,
			&vars->img.bits_per_pixel, &vars->img.line_length,
			&vars->img.endian);
}

int	main(int argc, char **argv)
{
	t_vars	vars;

	struct_init(&vars.ts);
	if (argc == 2)
	{
		vars.ts.filename = argv[1];
		parsing(&vars);
		print_map(vars.ts);
		vars.win = mlx_new_window(vars.mlx, vars.ts.r[0], vars.ts.r[1],
				"Cub3d");
		mlx_hook(vars.win, 2, 1L << 0, key_pressed, &vars);
		mlx_hook(vars.win, 3, 1L << 1, key_released, &vars);
		mlx_hook(vars.win, 17, 0L, &shutdown, &vars);
		create_img(&vars);
		mlx_loop_hook(vars.mlx, render_next_frame, &vars);
		mlx_loop(vars.mlx);
	}
	else
		printf("Error\nWrong number of arguments.\n");
	return (0);
}
