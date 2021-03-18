/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:14:42 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/18 16:09:17 by acusanno         ###   ########lyon.fr   */
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
		vars->tp.pa += M_PI / 12;
	if (keycode == RIGHT)
		vars->tp.pa -= M_PI / 12;
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
		vars->tp.pa = 2 * M_PI;
	else if (vars->tp.pa > (2 * M_PI))
		vars->tp.pa = 0;
	vars->tp.pdx = cos(vars->tp.pa) * 5;
	vars->tp.pdy = -sin(vars->tp.pa) * 5;
	find_inter(vars);
	//raycast(vars);
	printf("Keycode = %d\n", keycode);
	(void)vars;
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
	//mlx_clear_window(vars->mlx, vars->win);
	if (vars->minimap_display == 1)
	{
		map_draw(vars);
		my_mlx_pixel_put(&vars->img, vars->tp.x * vars->minimap_size, vars->tp.y * vars->minimap_size, 0x00FF0000);
		my_mlx_pixel_put(&vars->img, vars->tp.x * vars->minimap_size + vars->tp.pdx,
			vars->tp.y * vars->minimap_size + vars->tp.pdy, 0x00FF00FF);
		my_mlx_pixel_put(&vars->img, vars->tv.rx, vars->tv.ry, 0x00FF00FF);
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	return (0);
}

t_point	inter_line_v(t_vars *vars, t_point player, t_point cam, int line)
{
	t_point	inter;
	t_point	un;
	t_point	deux;

	un.x = vars->tl.v[line][0].x;
	un.y = vars->tl.v[line][0].y;
	deux.x = vars->tl.v[line][1].x;
	deux.y = vars->tl.v[line][1].y;
	inter.x = (deux.x * un.y - un.x * deux.y) * (cam.y - player.x);
	inter.x -= (cam.x * player.y - player.x * cam.y) * (deux.x - un.x);
	inter.x /= (deux.x - un.x) * (cam.y - player.y)
		- (cam.x - player.x) * (deux.y - un.y);
	inter.y = (deux.x * un.y - un.x * deux.y) * (cam.y - player.y);
	inter.y -= (cam.x * player.y - player.x * cam.y) * (deux.x - un.x);
	inter.y /= (deux.x - un.x) * (cam.y - player.y)
		- (cam.x - player.x) * (deux.y - un.y);
	return (inter);
}

t_point	inter_line_h(t_vars *vars, t_point player, t_point cam, int line)
{
	t_point	inter;
	t_point	un;
	t_point	deux;

	un.x = vars->tl.h[line][0].x;
	un.y = vars->tl.h[line][0].y;
	deux.x = vars->tl.h[line][1].x;
	deux.y = vars->tl.h[line][1].y;
	inter.x = (deux.x * un.y - un.x * deux.y) * (cam.y - player.x);
	inter.x -= (cam.x * player.y - player.x * cam.y) * (deux.x - un.x);
	inter.x /= (deux.x - un.x) * (cam.y - player.y)
		- (cam.x - player.x) * (deux.y - un.y);
	inter.y = (deux.x * un.y - un.x * deux.y) * (cam.y - player.y);
	inter.y -= (cam.x * player.y - player.x * cam.y) * (deux.x - un.x);
	inter.y /= (deux.x - un.x) * (cam.y - player.y)
		- (cam.x - player.x) * (deux.y - un.y);
	return (inter);
}

void	find_inter(t_vars *vars)
{
	t_point	player;
	t_point	cam;
	int		line_h;

	player.x = vars->tp.x;
	player.y = vars->tp.y;
	cam.x = vars->tp.pdx;
	cam.y = vars->tp.pdy;
	if (vars->tp.pa < M_PI)
		line_h = (int)player.y;
	else if (vars->tp.pa < M_PI)
		line_h = (int)player.y + 1;
	else
		line_h = 0;
	inter_line_h(vars, player, cam, line_h);
}

void	lines_init(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->ts.map_height)
	{
		vars->tl.v[i][0].x = 0;
		vars->tl.v[i][0].y = i;
		vars->tl.v[i][1].x = 1;
		vars->tl.v[i][1].y = i;
		i++;
	}
	i = 0;
	while (i < vars->ts.map_height)
	{
		vars->tl.v[i][0].x = i;
		vars->tl.v[i][0].y = 0;
		vars->tl.v[i][1].x = i;
		vars->tl.v[i][1].y = 1;
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

void	raycast(t_vars *vars)
{
	int		r;
	r = 0;
	vars->tv.ra = vars->tp.pa;
	while (r < vars->tv.r)
	{
		vars->tv.dof = 0;
		if (vars->tv.ra < M_PI)
		{
			
		}
		else if (vars->tv.ra > M_PI)
		{
			
		}
		else if (vars->tv.ra == 0 || vars->tv.ra == M_PI)
		{
			vars->tv.rx = vars->tp.x;
			vars->tv.ry = vars->tp.y;
			vars->tv.dof = 8;
		}
		while (vars->tv.dof < 8)
		{
			if (raytouch(vars, vars->tv.rx, vars->tv.ry) == 1)
				vars->tv.dof = 8;
			else if (raytouch(vars, vars->tv.rx, vars->tv.ry) == 0)
			{
				vars->tv.rx += vars->tv.xo;
				vars->tv.ry += vars->tv.yo;
				vars->tv.dof++;
			}
			else if (raytouch(vars, vars->tv.rx, vars->tv.ry) == -1)
			{
				vars->tv.rx = vars->tp.x;
				vars->tv.ry = vars->tp.y;
				vars->tv.dof = 8;
			}
		}
		r++;
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
		vars.minimap_display = 1;
		vars.run = 0;
		vars.ts.filename = argv[1];
		vars.tv.r = 1;
		vars.mlx = mlx_init();
		parse_settings(&vars.ts);
		settings_check(&vars);
		map_check(&vars.ts);
		dprintf(1, "yo");
		map_size(&vars.ts);
		dprintf(1, "yo2");
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
		while (vars.ts.map[i])
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
