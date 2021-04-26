/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 11:02:53 by acusanno          #+#    #+#             */
/*   Updated: 2021/04/23 10:44:53 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse_r(t_vars *vars, char *str)
{
	int		i;
	char	**array;

	if (vars->ts.r[0] != 0 || vars->ts.r[1] != 0)
	{
		printf("Error\n Resolution defined multiple times\n");
		ft_exit(-1, vars, str);
	}
	array = ft_split(str, ' ');
	i = ft_strlen_split(array);
	if (i != 3)
	{
		printf("Error\n Wrong number of resolution arguments\n");
		ft_exit(-1, vars, str);
	}
	vars->ts.r[0] = ft_atoi(array[1]);
	vars->ts.r[1] = ft_atoi(array[2]);
	free_tab(array);
	if (vars->ts.r[0] <= 0 || vars->ts.r[1] <= 0)
	{
		printf("Error\n Wrong resolution arguments\n");
		ft_exit(-1, vars, str);
	}
}

void	parse_nswes(t_vars *vars, char *str)
{
	int		i;
	char	**array;

	array = ft_split(str, ' ');
	i = ft_strlen_split(array);
	if (i != 2)
	{
		printf("Error\n Wrong number of arguments");
		ft_exit(-1, vars, str);
	}
	if (array[0][0] == 'N' && !vars->ts.no)
		vars->ts.no = ft_strdup(array[1]);
	else if (array[0][0] == 'S' && array[0][1] == 'O' && !vars->ts.so)
		vars->ts.so = ft_strdup(array[1]);
	else if (array[0][0] == 'W' && !vars->ts.we)
		vars->ts.we = ft_strdup(array[1]);
	else if (array[0][0] == 'E' && !vars->ts.ea)
		vars->ts.ea = ft_strdup(array[1]);
	else if (array[0][0] == 'S' && !vars->ts.s)
		vars->ts.s = ft_strdup(array[1]);
	else
	{
		printf("Error\n Textures defined multiple times\n");
		ft_exit(-1, vars, str);
	}
	free_tab(array);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	parse_fc(t_vars *vars, char *str)
{
	int		i;
	char	**array;
	char	**tab;

	array = ft_split(str, ' ');
	tab = ft_split(array[1], ',');
	i = ft_strlen_split(tab);
	if (i != 3)
	{
		printf("Error\n Wrong number of color arguments\n");
		ft_exit(-1, vars, str);
	}
	else if (check_fc(tab) == -1)
		ft_exit(-1, vars, str);
	if ((vars->ts.f != -1 && array[0][0] == 'F')
		|| (vars->ts.c != -1 && array[0][0] == 'C'))
	{
		printf("Error\n Floor or Ceiling defined multiple times\n");
		ft_exit(-1, vars, str);
	}
	put_tab(tab, &vars->ts, array[0][0]);
	free_tab(array);
	free_tab(tab);
	return (0);
}

void	parse_map(t_vars *vars, char *str)
{
	int		i;
	int		j;

	i = 1;
	j = 0;
	vars->ts.map = malloc(sizeof(char *));
	vars->ts.map[0] = ft_strdup(str);
	while (get_next_line(vars->ts.fd, &str) != 0)
	{
		if (str[0])
		{
			if (j == 0)
			{
				map_realloc(&vars->ts, str, i);
				free(str);
				i++;
			}
			else
			{
				printf("Error\n Map invalid");
				ft_exit(-1, vars, str);
			}
		}
		else
		{
			free(str);
			j++;
		}
	}
	if (str[0])
	{
		if (j == 0)
			map_realloc(&vars->ts, str, i);
		else
		{
			printf("Error\n Map invalid");
			ft_exit(-1, vars, str);
		}
	}
	free(str);
}

void	parse_settings(t_vars *vars)
{
	char	*str;

	str = NULL;
	vars->ts.fd = open(vars->ts.filename, O_RDONLY);
	while ((get_next_line(vars->ts.fd, &str)) != 0)
	{
		if (*str)
		{
			if (*str == 'R')
				parse_r(vars, str);
			else if (*str == 'N' || *str == 'S' || *str == 'W' || *str == 'E')
				parse_nswes(vars, str);
			else if (*str == 'F' || *str == 'C')
				parse_fc(vars, str);
			else if (*str == ' ' || *str == '1')
			{
				parse_map(vars, str);
				free(str);
				return ;
			}
			else
			{
				printf("Error\n Settings invalid");
				ft_exit(-1, vars, str);
			}
		}
		free(str);
	}
	free(str);
}
