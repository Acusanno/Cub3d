/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 12:34:53 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/21 13:43:14 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	header_bmp(t_vars *vars, int fd)
{
	int	file_size;

	file_size = 54 + 24 * vars->ts.r[0] * vars->ts.r[1];
	write(fd, "BM", 2);
	write(fd, &file_size, 4);
	file_size = 0;
	write(fd, &file_size, 4);
	file_size = 54;
	write(fd, &file_size, 4);
	file_size = 40;
	write(fd, &file_size, 4);
	file_size = vars->ts.r[0];
	write(fd, &file_size, 4);
	file_size = vars->ts.r[1];
	write(fd, &file_size, 4);
	file_size = 1;
	write(fd, &file_size, 2);
	write(fd, &vars->img.bits_per_pixel, 2);
	file_size = 0;
	write(fd, &file_size, 4);
	write(fd, &file_size, 4);
	write(fd, &file_size, 4);
	write(fd, &file_size, 4);
	write(fd, &file_size, 4);
	write(fd, &file_size, 4);
}

void	img_to_bitmap(t_vars *vars, int fd)
{
	int	i;
	int	j;
	int	temp;

	i = vars->ts.r[1] - 1;
	while (i > 0)
	{
		j = 0;
		while (j < vars->ts.r[0])
		{
			temp = vars->img.iaddr[i * vars->img.line_length / 4 + j];
			write(fd, &temp, 4);
			j++;
		}
		i--;
	}
}

void	bitmap_create(t_vars *vars)
{
	int	fd;

	fd = open("screen.bmp", O_CREAT | O_RDWR, 0777);
	if (fd == -1)
	{
		printf("Error\nCouldn't create screen.bmp.\n");
		ft_exit(-1, vars, NULL);
	}
	header_bmp(vars, fd);
	img_to_bitmap(vars, fd);
}

int	render_bitmap(t_vars *vars)
{
	update_player_pos(vars);
	dist_center_sprite(vars);
	sprite_ordering(vars);
	find_all_inter(vars);
	draw_screen(vars);
	return (0);
}

void	full_bitmap(t_vars *vars)
{
	parsing(vars);
	print_map(vars->ts);
	vars->img.img = mlx_new_image(vars->mlx, vars->ts.r[0], vars->ts.r[1]);
	vars->img.addr = mlx_get_data_addr(vars->img.img,
			&vars->img.bits_per_pixel, &vars->img.line_length,
			&vars->img.endian);
	vars->img.iaddr = (int *)mlx_get_data_addr(vars->img.img,
			&vars->img.bits_per_pixel, &vars->img.line_length,
			&vars->img.endian);
	render_bitmap(vars);
	bitmap_create(vars);
}