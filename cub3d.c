/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:14:42 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/24 16:52:05 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "math.h"

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
	find_inter(vars);
	printf("Keycode = %d\n", keycode);
	printf("Inter_h.x = %f\n", vars->tp.inter_h.x);
	printf("Inter_h.y = %f\n", vars->tp.inter_h.y);
	printf("Inter_v.x = %f\n", vars->tp.inter_v.x);
	printf("Inter_v.y = %f\n", vars->tp.inter_v.y);
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
		if (vars->tp.inter_h.z != -1 && vars->tp.inter_h.x < vars->ts.map_width
			&& vars->tp.inter_h.x > 0)
			my_mlx_pixel_put(&vars->img, vars->tp.inter_h.x * vars->minimap_size,
				vars->tp.inter_h.y * vars->minimap_size, 0x00FF00FF);
		if (vars->tp.inter_v.z != -1 && vars->tp.inter_v.x < vars->ts.map_width
			&& vars->tp.inter_v.x > 0 && vars->tp.inter_v.y >= 0)
			my_mlx_pixel_put(&vars->img, vars->tp.inter_v.x * vars->minimap_size,
				vars->tp.inter_v.y * vars->minimap_size, 0x00FF00FF);
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	return (0);
}

double	line_slope(float a1, float a2, float b1, float b2)
{
	if (a1 - b1 == 0.0)
		return (NAN);
	else
		return ((a2 - b2) / (a1 - b1));
}

void	set_x_y(t_point *res, double first, double second)
{
	res->x = first;
	res->y = second;
}

int		intersection(t_vars *vars, t_lines line, t_point *res)
{
	double	slope_a;
	double	slope_b;
	t_pixel	a1;

	res->z = 0;
	a1 = vars->tp;
	slope_a = line_slope(a1.x, a1.y, a1.pdx + a1.x, a1.pdy + a1.y);
	slope_b = line_slope(line.a.x, line.a.y, line.b.x, line.b.y);
	if (slope_a == slope_b || (isnan(slope_a) && isnan(slope_b)))
	{
		res->z = -1;
		return (-1);
	}
	else if (isnan(slope_a) && !isnan(slope_b))
		set_x_y(res, a1.x, (a1.x - line.a.x) * slope_b + line.a.y);
	else if (isnan(slope_b) && !isnan(slope_a))
		set_x_y(res, line.a.x, (line.a.x - a1.x) * slope_a + a1.y);
	else
		set_x_y(res, (slope_a * a1.x - slope_b * line.a.x + line.a.y - a1.y) \
	    / (slope_a - slope_b), slope_b * (res->x - line.a.x) + line.a.y);
	return (1);
}

void	map_transform(t_vars *vars)
{
	int		i;
	int		j;
	char	**newmap;

	i = 0;
	newmap = malloc(sizeof(char *) * vars->ts.map_height + 1);
	while (vars->ts.map[i])
	{
		newmap[i] = malloc(sizeof(char) * vars->ts.map_width + 1);
		j = 0;
		while (j < vars->ts.map_width)
		{
			if (j < (int)ft_strlen(vars->ts.map[i]))
				newmap[i][j] = vars->ts.map[i][j];
			else
				newmap[i][j] = ' ';
			j++;
		}
		newmap[i][j] = '\0';
		i++;
	}
	free(vars->ts.map);
	vars->ts.map = newmap;
}

int	check_wall(t_vars *vars, float x, float y, char tal)
{
	int	i;

	i = 0;
	if (tal == 'h' && vars->tp.pa < M_PI)
		i = 1;
	else if (tal == 'v' && vars->tp.pa > M_PI / 2
		&& vars->tp.pa < 3 * M_PI / 2)
		i = 1;
	if (x < 0 || y < 0 || x > vars->ts.map_width
		|| y > vars->ts.map_height)
	{
		if (tal == 'h')
			vars->tp.inter_h.z = -1;
		else
			vars->tp.inter_v.z = -1;
		return (-1);
	}
	if (tal == 'h' && vars->ts.map[(int)y - i][(int)x] == '1')
		return (1);
	if (tal == 'v' && vars->ts.map[(int)y][(int)x - i] == '1')
		return (1);
	return (0);
}

void	find_inter(t_vars *vars)
{
	int	i;

	i = (int)vars->tp.y;
	while (i && i < vars->ts.map_height)
	{
		intersection(vars, vars->tg.h[i], &(vars->tp.inter_h));
		if (check_wall(vars, vars->tp.inter_h.x, vars->tp.inter_h.y, 'h') != 0)
			break ;
		if (vars->tp.pa < M_PI)
			i--;
		else
			i++;
	}
	i = (int)vars->tp.x;
	while (i && i < vars->ts.map_width)
	{
		intersection(vars, vars->tg.v[i], &(vars->tp.inter_v));
		if (check_wall(vars, vars->tp.inter_v.x, vars->tp.inter_v.y, 'v') != 0)
			break ;
		if (vars->tp.pa < M_PI / 2 || vars->tp.pa > 3 * M_PI / 2)
			i++;
		else
			i--;
	}
}

// void	find_inter(t_vars *vars)
// {
// 	int	i;

// 	i = (int)vars->tp.y;
// 	vars->tp.inter_h.x = vars->tp.x;
// 	vars->tp.inter_h.y = vars->tp.y;
// 	if (vars->tp.pa < M_PI)
// 	{
// 		while (i)
// 		{
// 			intersection(vars, vars->tg.h[i], &(vars->tp.inter_h));
// 			if (check_wallh(vars, vars->tp.inter_h.x, vars->tp.inter_h.y) != 0)
// 				break ;
// 			i--;
// 		}
// 	}
// 	else if (vars->tp.pa > M_PI)
// 	{
// 		i++;
// 		while (i < vars->ts.map_height)
// 		{
// 			intersection(vars, vars->tg.h[i], &(vars->tp.inter_h));
// 			if (check_wallh(vars, vars->tp.inter_h.x, vars->tp.inter_h.y) != 0)
// 				break ;
// 			i++;
// 		}
// 	}
// 	else if (vars->tp.pa == M_PI || vars->tp.pa == 0)
// 	{
// 		vars->tp.inter_h.x = vars->tp.x;
// 		vars->tp.inter_h.y = vars->tp.y;
// 	}
// 	i = (int)vars->tp.x;
// 	vars->tp.inter_v.x = vars->tp.x;
// 	vars->tp.inter_v.y = vars->tp.y;
// 	if (vars->tp.pa > M_PI / 2 && vars->tp.pa < 3 * M_PI / 2)
// 	{
// 		while (i)
// 		{
// 			intersection(vars, vars->tg.v[i], &(vars->tp.inter_v));
// 			if (vars->tp.inter_v.y < 0 || vars->tp.inter_v.x < 0
// 				|| check_wallv(vars, vars->tp.inter_v.x, vars->tp.inter_v.y)
// 				 != 0)
// 				break ;
// 			i--;
// 		}
// 	}
// 	else if ((vars->tp.pa < M_PI / 2 && vars->tp.pa > 0)
// 		|| (vars->tp.pa > 3 * M_PI / 2 && vars->tp.pa < 2 * M_PI))
// 	{
// 		i++;
// 		while (i < vars->ts.map_width)
// 		{
// 			intersection(vars, vars->tg.v[i - 1], &(vars->tp.inter_v));
// 			if (vars->tp.inter_v.y < 0 || vars->tp.inter_v.x < 0
// 				|| check_wallv(vars, vars->tp.inter_v.x, vars->tp.inter_v.y)
// 				 != 0)
// 				break ;
// 			i++;
// 		}
// 	}
// 	else if (vars->tp.pa == M_PI / 2 || vars->tp.pa == 3 * M_PI / 2)
// 	{
// 		vars->tp.inter_v.x = vars->tp.x;
// 		vars->tp.inter_v.y = vars->tp.y;
// 	}
// }

void	lines_init(t_vars *vars)
{
	int	i;

	vars->tg.h = ft_calloc(vars->ts.map_height + 1, sizeof(t_lines));
	vars->tg.v = ft_calloc(vars->ts.map_width + 1, sizeof(t_lines));
	i = 0;
	while (i < vars->ts.map_height)
	{
		vars->tg.h[i].a.x = 0;
		vars->tg.h[i].a.y = i;
		vars->tg.h[i].b.x = 1;
		vars->tg.h[i].b.y = i;
		i++;
	}
	i = 0;
	while (i < vars->ts.map_width)
	{
		vars->tg.v[i].a.x = i;
		vars->tg.v[i].a.y = 0;
		vars->tg.v[i].b.x = i;
		vars->tg.v[i].b.y = 1;
		i++;
	}
}

void	spawn_init(t_vars *vars)
{
	if (vars->ts.spawn == 'N')
		vars->tp.pa = (M_PI / 2);
	if (vars->ts.spawn == 'S')
		vars->tp.pa = ((3 * M_PI) / 2);
	if (vars->ts.spawn == 'E')
		vars->tp.pa = 0;
	if (vars->ts.spawn == 'O')
		vars->tp.pa = M_PI;
}

int	raytouch(t_vars *vars, int x, int y)
{
	x /= vars->minimap_size;
	y /= vars->minimap_size;
	if (y >= vars->ts.map_width || x >= vars->ts.map_height)
		return (-1);
	else if (x < 0 || y < 0)
		return (-1);
	else if (vars->ts.map[x][y] == '1')
		return (1);
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
		printf("Error\nNombre d'arguments incorrect\n");
	vars.win = mlx_new_window(vars.mlx, vars.ts.r[0], vars.ts.r[1], "Cub3d");
	spawn_player(&vars);
	spawn_init(&vars);
	lines_init(&vars);
	vars.tp.pdx = cos(vars.tp.pa) * 5;
	vars.tp.pdy = -sin(vars.tp.pa) * 5;
	mlx_hook(vars.win, 2, 1L << 0, key_hook, &vars);
	mlx_hook(vars.win, 17, 0L, &shutdown, &vars);
	vars.img.img = mlx_new_image(vars.mlx, vars.ts.r[0], vars.ts.r[1]);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel,
			&vars.img.line_length, &vars.img.endian);
	mlx_loop_hook(vars.mlx, render_next_frame, &vars);
	mlx_loop(vars.mlx);
	return (0);
}

// int	check_wallh(t_vars *vars, float x, float y)
// {
// 	int test = 0;

// 	if (vars->tp.pa < M_PI && vars->tp.pa > 0)
// 		test = 1;
// 	if (y >= ft_strlen_split(vars->ts.map)
// 		|| x >= (int)ft_strlen(vars->ts.map[(int)y])
// 		|| x < 0 || y - test <= 0)
// 	{
// 		vars->tp.inter_h.z = -1;
// 		return (-1);
// 	}
// 	else if (x < 0 || x > vars->ts.map_width)
// 		return (-1);
// 	else if (vars->ts.map[(int)(y - test)][(int)x] == '1')
// 		return (1);
// 	return (0);
// }

// int	check_wallv(t_vars *vars, float x, float y)
// {
// 	int test = 0;

// 	if (vars->tp.pa > M_PI / 2 && vars->tp.pa < 3 * M_PI / 2)
// 		test = 1;
// 	if (y > ft_strlen_split(vars->ts.map)
// 		|| x >= (int)ft_strlen(vars->ts.map[(int)y])
// 		|| x < 0 || y < 0 || x > vars->ts.map_width)
// 	{
// 		vars->tp.inter_v.z = -1;
// 		vars->tp.inter_v.x = 0;
// 		vars->tp.inter_v.y = 0;
// 		return (-1);
// 	}
// 	else if (vars->ts.map[(int)y][(int)(x - test)] == '1')
// 		return (1);
// 	return (0);
// }

/*int main()
{
	int	worldMap[24][24]=
	{
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	t_vars	vars;
	double posX = 22, posY = 12;
	double dirX = -1, dirY = 0;
	double planeX = 0, planeY = 0.66;

	double time = 0;
	double oldTime = 0;
	double frameTime;

	double cameraX;
	double rayDirX;
	double rayDirY;

	int	mapX;
	int mapY;

	double sideDistX;
	double sideDistY;

	double deltaDistX;
	double deltaDistY;
	double perpWallDist;

	int stepX;
	int stepY;

	int hit;
	int side;

	int lineHeight;

	int drawStart;
	int drawEnd;

	double moveSpeed;
	double rotSpeed;

	int color;

	int x;
	double w;
	double h;

	double oldDirX;
	double oldPlaneX;

	hit = 0;
	x = 0;
	screen(640, 480, 0, "Raycaster");
	h = 480;
	w = 640;
	while (1)
	{
		while (x < w)
		{
			cameraX = 2 * x / w - 1;
			rayDirX = dirX + planeX * cameraX;
			rayDirY = dirY + planeY * cameraX;

			mapX = (int)posX;
			mapY = (int)posY;
	
			if (rayDirY == 0)
				deltaDistX = 0;
			else if (rayDirX == 0)
				deltaDistX = 1;
			else
				deltaDistX = (fabs(1 / rayDirX));
			if (rayDirX == 0)
				deltaDistY = 0;
			else if (rayDirY == 0)
				deltaDistY = 1;
			else
				deltaDistY = (fabs(1 / rayDirX));
			x++;

			if (rayDirX < 0)
			{
				stepX = -1;
				sideDistX = (posX - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX + 1.0 - posX);
			}
			if (rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (posY - mapY) * deltaDistY;			
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY + 1.0 - posY) * deltaDistY;
			}
			while (hit == 0)
			{
				if (sideDistX < sideDistY)
				{
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else
				{
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}
				if (worldMap[mapX][mapY] > 0)
					hit = 1;
			}
			if (side == 0)
				perpWallDist = ((mapX - posX + (1 - stepX) / 2) / rayDirX);
			else
				perpWallDist = ((mapY - posY + (1 - stepY) / 2) / rayDirY);
			
			lineHeight = (int)(h / perpWallDist);
			drawStart = (-lineHeight / 2) + (h / 2);
			if (drawStart < 0)
				drawStart = 0;
			drawEnd = (lineHeight / 2) + (h / 2);
			if (drawEnd >= h)
				drawEnd = h - 1;
			if (worldMap[mapX][mapY] == 1)
				color = 255;
			else if (worldMap[mapX][mapY] == 2)
				color = 200;
			else if (worldMap[mapX][mapY] == 3)
				color = 150;
			else if (worldMap[mapX][mapY] == 4)
				color = 100;
			else 
				color = 50;

			if (side == 1)
			{
				color = color / 2;
			}

			verLine(x, drawStart, drawEnd, color);

			oldTime = time;
			time = getTicks();
			frameTime = (time - oldTime) / 1000.0;
			print(1.0 / frameTime);
			redraw();
			cls();

			moveSpeed = frameTime * 5.0;
			rotSpeed = frameTime * 3.0;

			readKeys();

    		if (keyDown(UP))
    		{
    			if(worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)] == 0)
					posX += dirX * moveSpeed;
    			if(worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)] == 0)
					posY += dirY * moveSpeed;
    		}
    		if (keyDown(DOWN))
    		{
    			if(worldMap[(int)(posX - dirX * moveSpeed)][(int)(posY)] == 0)
					posX -= dirX * moveSpeed;
    			if(worldMap[(int)(posX)][(int)(posY - dirY * moveSpeed)] == 0)
					posY -= dirY * moveSpeed;
    		}
    		if (keyDown(RIGHT))
    		{
    			oldDirX = dirX;
    			dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
    			dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
    			oldPlaneX = planeX;
    			planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
    			planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    		}
    		if (keyDown(LEFT))
    		{
    			oldDirX = dirX;
    			dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
    			dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
    			oldPlaneX = planeX;
    			planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
    			planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
			}
		}
	}
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, 640, 480, "Hello world!");
	mlx_key_hook(vars.win, key_hook, &vars);
	mlx_loop_hook(vars.mlx, render_next_frame, struct);
	mlx_loop(vars.mlx);
	return (0);
}*/
