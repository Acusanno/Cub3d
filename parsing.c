/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 11:02:53 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/28 10:28:24 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse_nswes(t_vars *vars, char *str, int i)
{
	char	**array;

	array = ft_split(str, ' ');
	i = ft_strlen_split(array);
	if (i != 2 || ft_strlen(array[0]) > 2)
	{
		printf("Error\n Wrong number of arguments");
		ft_exit(-1, str);
	}
	if (array[0][0] == 'N' && !vars->ts.no)
		vars->ts.no = ft_strdup(array[1]);
	else if (array[0][0] == 'S' && array[0][1] == 'O' && !vars->ts.so)
		vars->ts.so = ft_strdup(array[1]);
	else if (array[0][0] == 'W' && !vars->ts.we)
		vars->ts.we = ft_strdup(array[1]);
	else if (array[0][0] == 'E' && !vars->ts.ea)
		vars->ts.ea = ft_strdup(array[1]);
	else if (array[0][0] == 'S' && !vars->ts.s && ft_strlen(array[0]) == 1)
		vars->ts.s = ft_strdup(array[1]);
	else
	{
		printf("Error\n Textures defined multiple times\n");
		ft_exit(-1, str);
	}
	free_tab(array);
}

int	parse_fc(t_vars *vars, char *str)
{
	int		i;
	char	**array;
	char	**tab;

	array = ft_split(str, ' ');
	tab = ft_split(array[1], ',');

	i = ft_strlen_split(tab);
	if (i != 3 || ft_strlen(array[0]) > 1)
	{
		printf("Error\n Wrong number of color arguments\n");
		ft_exit(-1, str);
	}
	else if (check_fc(tab) == -1)
		ft_exit(-1, str);
	if ((vars->ts.f != -1 && array[0][0] == 'F')
		|| (vars->ts.c != -1 && array[0][0] == 'C'))
	{
		printf("Error\n Floor or Ceiling defined multiple times\n");
		ft_exit(-1, str);
	}
	put_tab(tab, &vars->ts, array[0][0]);
	free_tab(tab);
	free_tab(array);
	return (0);
}

void	parse_line(t_vars *vars, char *str, int i)
{
	if (vars->ts.blank_line == 0)
	{
		map_realloc(&vars->ts, str, i);
		free(str);
	}
	else
	{
		printf("Error\n Map invalid");
		ft_exit(-1, str);
	}
}

void	parse_map(t_vars *vars, char *str)
{
	int		i;

	i = 1;
	vars->ts.blank_line = 0;
	vars->ts.map = malloc(sizeof(char *));
	vars->ts.map[0] = ft_strdup(str);
	while (get_next_line(vars->ts.fd, &str) > 0)
	{
		if (str[0])
		{
			parse_line(vars, str, i);
			i++;
		}
		else
		{
			free(str);
			vars->ts.blank_line++;
		}
	}
	if (str[0])
		parse_line(vars, str, i);
}

void	parse_settings(t_vars *vars)
{
	char	*str;

	str = NULL;
	vars->ts.fd = open(vars->ts.filename, O_RDONLY);
	if (vars->ts.fd == -1 || ft_strlen(vars->ts.filename) <= 4
		|| ft_strncmp(".cub", &vars->ts.filename
			[ft_strlen(vars->ts.filename) - 4], 4) != 0)
	{
		printf("Error\n Cub file invalid");
		ft_exit(-1, NULL);
	}
	while ((get_next_line(vars->ts.fd, &str)) > 0)
		character_check(vars, str);
	free(str);
}
