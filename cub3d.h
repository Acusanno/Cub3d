/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 10:10:25 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/19 14:40:51 by acusanno         ###   ########lyon.fr   */
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
# define CTRL 256
# define RED "ff0000"
# define GREEN "00ff00"
# define BLUE "0000ff"
# define CYAN "00ffff"
# define WHITE "ffffff"

typedef struct s_point {
	float			x;
	float			y;
	int				z;
}					t_point;

typedef struct s_settings
{
	int				fd;
	char			*filename;
	int				r[2];
	char			spawn;
	t_point			*sprite;
	float			*dist_center_sp;
	int				nb_sp;
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

typedef struct s_data {
	void			*img;
	char			*addr;
	int				*iaddr;
	char			*path;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				img_width;
	int				img_height;
	int				text_x;
	int				text_y;
}					t_data;

typedef struct s_pixel {
	float	x;
	float	y;
	t_point	player;
	float	pdx;
	float	pdy;
	float	pa;
	float	ra;
	int		ri;
	int		color;
	int		start;
	int		end;
	t_point	*inter_h;
	t_point	*inter_v;
	t_point	**inter_s;
	int		*vis_sp;
	char	*face;
	float	*dist;
	float	**dist_sp;
	int		step_x;
	int		step_y;
}					t_pixel;

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
	int				ctrl;
}					t_controls;

typedef struct s_vars {
	void			*mlx;
	void			*win;
	int				minimap_size;
	t_data			img;
	t_pixel			tp;
	t_settings		ts;
	t_grid			tg;
	t_controls		tc;
	t_data			*td;
}					t_vars;

// Parsing du fichier de config
void				struct_init(t_settings *ts);
int					ft_strlen_split(char **str);
int					check_fc(char **array);
void				put_tab(char **tab, t_settings *ts, char c);
void				map_realloc(t_settings *ts, char *str, int i);
void				parse_r(t_vars *vars, char *str);
void				parse_nswes(t_vars *vars, char *str);
int					parse_fc(t_vars *vars, char *str);
void				parse_map(t_vars *vars, char *str);
void				parse_settings(t_vars *vars);
void				map_transform(t_vars *vars);
void				free_tab(char **tab);

// Verification de validité de la map
void				check_space(t_settings *ts, int i, int j);
void				map_check(t_settings *ts);
void				map_size(t_settings *ts);

// Verification de validité des parametres
void				settings_check(t_vars *vars);

// Affichage de la map
void				draw_cube(int x, int y, int color, t_vars *vars);
void				map_draw(t_vars *vars);
void				spawn_player(t_vars *vars);
int					is_spawn(char c);

// Gestion des touches et des movements
int					key_pressed(int keycode, t_vars *vars);
int					key_released(int keycode, t_vars *vars);
void				rotation(t_vars *vars);
void				update_player_pos(t_vars *vars);

// Création grille de la map
void				lines_init(t_vars *vars);

// Intersection avec grille
t_point				inter_line_v(t_vars *vars, t_point player,
						t_point cam, int line);
t_point				inter_line_h(t_vars *vars, t_point player,
						t_point cam, int line);
float				dist(t_point a, t_point b);

// Calculs
t_point				intersection(t_point a1, t_point a2, t_point b1,
						t_point b2);
double				line_slope(float a1, float a2, float b1, float b2);
void				set_x_y(t_point *res, double first, double second);
void				raycast(t_vars *vars);
int					raytouch(t_vars *vars, int x, int y);

// Mallocs et initialisations
void				spawn_init(t_vars *vars);
void				inter_malloc(t_vars *vars);
void				inter_init(t_vars *vars, int ri);
void				controls_init(t_vars *vars);

//	Calcul d'intersection des murs et des sprites
int					wall_h(t_vars *vars, float x, float y);
int					wall_v(t_vars *vars, float x, float y);
void				find_inter_h(t_vars *vars);
void				find_inter_v(t_vars *vars);
void				find_inter_s(t_vars *vars, int sprite);
int					check_wall(t_vars *vars, float x, float y, char tal);
int					which_sprite(t_vars *vars, float x, float y);
void				sprite_check(t_settings *ts);
void				distance_comp(t_vars *vars, int ri);
void				find_all_inter(t_vars *vars);

// Textures
void				read_img(t_vars *vars, int i);
void				read_all_img(t_vars *vars);
int					texture_index(float angle, char face);
void				put_path(t_vars *vars);
void				text_x_wall(t_vars *vars, int j, int ri);
void				text_x_sprite(t_vars *vars, int sprite, int ri);

// Tri des sprites
void				sprite_init(t_vars *vars);
void				dist_center_sprite(t_vars *vars);
void				sprite_ordering(t_vars *vars);

// Affichage murs et sprites
void				my_mlx_pixel_put(t_vars *vars, int x, int y, int color);
void				start_end_init(t_vars *vars, float ratio_height);
void				draw_column(t_vars *vars, int ri, float ratio_height,
						int j);
void				draw_sprite(t_vars *vars, int ri, float ratio_height,
						int sprite);
void				draw_screen(t_vars *vars);
int					render_next_frame(t_vars *vars);

// Fermer la fenêtre
void				ft_exit(int code, t_vars *vars, char *truc);
int					shutdown(int keycode, t_vars *vars);

#endif
