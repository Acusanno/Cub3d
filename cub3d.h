/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:10:25 by acusanno          #+#    #+#             */
/*   Updated: 2021/04/02 10:26:23 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include "mlx.h"
# include "libft/libft.h"
# define W 13
# define A 0
# define S 1
# define D 2
# define UP 126
# define DOWN 125
# define LEFT 123
# define RIGHT 124
# define ESC 53
# define TAB 48
# define SHIFT 257
# define RED "ff0000"
# define GREEN "00ff00"
# define BLUE "0000ff"
# define CYAN "00ffff"
# define WHITE "ffffff"

typedef struct s_settings
{
	int				fd;
	char			*filename;
	int				r[2];
	char			spawn;
	char			*no;
	char			*so;
	char			*we;
	char			*ea;
	char			*s;
	int				f;
	int				c;
	char			**map;
	int				map_width;
	int				map_height;
}					t_settings;

typedef struct s_point {
	float			x;
	float			y;
	float			z;
}					t_point;

typedef struct s_data {
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
}					t_data;

typedef struct s_pixel {
	float	x;
	float	y;
	float	pdx;
	float	pdy;
	float	pa;
	float	ra;
	int		ri;
	int		color;
	t_point	*inter_h;
	t_point	*inter_v;
}					t_pixel;

typedef struct s_rays {
	int				r;
	int				mx;
	int				my;
	int				mp;
	int				dof;
	float			rx;
	float			ry;
	float			ra;
	float			xo;
	float			yo;
}					t_rays;

typedef struct s_lines {
	t_point				a;
	t_point				b;
}					t_lines;

typedef struct s_grid {
	t_lines			*h;
	t_lines			*v;
}					t_grid;

typedef struct s_controls {
	int				w;
	int				a;
	int				s;
	int				d;
	int				left;
	int				right;
	int				tab;
	int				shift;
}					t_controls;

typedef struct s_vars {
	void			*mlx;
	void			*win;
	int				minimap_size;
	t_data			img;
	t_pixel			tp;
	t_settings		ts;
	t_rays			tv;
	t_grid			tg;
	t_controls		tc;
}					t_vars;

void				struct_init(t_settings *ts);
int					ft_strlen_split(char **str);
int					check_fc(char **array);
void				put_tab(char **tab, t_settings *ts, char c);
void				map_realloc(t_settings *ts, char *str, int i);
void				parse_r(t_settings *ts, char *str);
void				parse_nswes(t_settings *ts, char *str);
int					parse_fc(t_settings *ts, char *str);
void				parse_map(t_settings *ts, char *str);
void				parse_settings(t_settings *ts);
void				check_space(t_settings *ts, int i, int j);
void				map_check(t_settings *ts);
void				settings_check(t_vars *vars);
void				map_size(t_settings *ts);
void				draw_cube(int x, int y, int color, t_vars *vars);
void				map_draw(t_vars *vars);
void				my_mlx_pixel_put(t_data *data, int x, int y, int color);
int					is_spawn(char c);
void				spawn_player(t_vars *vars);
void				raycast(t_vars *vars);
void				lines_init(t_vars *vars);
void				spawn_init(t_vars *vars);
t_point				inter_line_v(t_vars *vars, t_point player,
						t_point cam, int line);
t_point				inter_line_h(t_vars *vars, t_point player,
						t_point cam, int line);
void				find_inter_h(t_vars *vars);
void				find_inter_v(t_vars *vars);
void				find_all_inter(t_vars *vars);
int					intersection(t_vars *vars, t_lines line, t_point *res);
int					raytouch(t_vars *vars, int x, int y);
void				map_transform(t_vars *vars);
void				set_x_y(t_point *res, double first, double second);
double				line_slope(float a1, float a2, float b1, float b2);
int					check_wall(t_vars *vars, float x, float y, char tal);

#endif
