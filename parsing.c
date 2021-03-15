/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 11:02:53 by acusanno          #+#    #+#             */
/*   Updated: 2021/03/15 13:38:19 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse_r(t_settings *ts, char *str)
{
	int		i;
	char	**array;

	if (ts->r[0] != 0 || ts->r[1] != 0)
	{
		printf("Error\n Resolution defined multiple times\n");
		exit(-1);
	}
	array = ft_split(str, ' ');
	i = ft_strlen_split(array);
	if (i != 3)
	{
		printf("Error\n Wrong number of resolution arguments\n");
		exit(-1);
	}
	ts->r[0] = ft_atoi(array[1]);
	ts->r[1] = ft_atoi(array[2]);
	if (ts->r[0] <= 0 || ts->r[1] <= 0)
	{
		printf("Error\n Wrong resolution arguments\n");
		exit(-1);
	}
}

void	parse_nswes(t_settings *ts, char *str)
{
	int		i;
	char	**array;

	array = ft_split(str, ' ');
	i = ft_strlen_split(array);
	if (i != 2)
	{
		printf("Error\n Wrong number of arguments");
		exit(-1);
	}
	if (array[0][0] == 'N' && !ts->no)
		ts->no = ft_strdup(array[1]);
	else if (array[0][0] == 'S' && array[0][1] == 'O' && !ts->so)
		ts->so = ft_strdup(array[1]);
	else if (array[0][0] == 'W' && !ts->we)
		ts->we = ft_strdup(array[1]);
	else if (array[0][0] == 'E' && !ts->ea)
		ts->ea = ft_strdup(array[1]);
	else if (array[0][0] == 'S' && !ts->s)
		ts->s = ft_strdup(array[1]);
	else
	{
		printf("Error\n Textures defined multiple times\n");
		exit(-1);
	}
}

int		parse_fc(t_settings *ts, char *str)
{
	int		i;
	char	**array;
	char	**tab;

	array = ft_split(str, ' ');
	tab = ft_split(array[1], ',');
	if ((i = ft_strlen_split(tab)) != 3)
	{
		printf("Error\n Wrong number of color arguments\n");
		exit(-1);
	}
	else if (check_fc(tab) == -1)
		exit(-1);
	if ((ts->f != -1 && array[0][0] == 'F') ||
	(ts->c != -1 && array[0][0] == 'C'))
	{
		printf("Error\n Floor or Ceiling defined multiple times\n");
		exit(-1);
	}
	put_tab(tab, ts, array[0][0]);
	return (0);
}

void	parse_map(t_settings *ts, char *str)
{
	int i;
	int j;

	i = 1;
	j = 0;
	ts->map = malloc(sizeof(char *));
	ts->map[0] = ft_strdup(str);
	while (get_next_line(ts->fd, &str) != 0)
	{
		if (str[0])
		{
			if (j == 0)
			{
				map_realloc(ts, str, i);
				i++;
			}
			else
			{
				printf("Error\n Map invalid");
				exit(-1);
			}
		}
		else
			j++;
	}
	if (str[0])
	{
		if (j == 0)
			map_realloc(ts, str, i);
		else
		{
			printf("Error\n Map invalid");
			exit(-1);
		}
	}
}

void	parse_settings(t_settings *ts)
{
	char	*str;

	str = NULL;
	ts->fd = open(ts->filename, O_RDONLY);
	while ((get_next_line(ts->fd, &str)) != 0)
	{
		if (*str)
		{
			if (*str == 'R')
				parse_r(ts, str);
			else if (*str == 'N' || *str == 'S' || *str == 'W' || *str == 'E')
				parse_nswes(ts, str);
			else if (*str == 'F' || *str == 'C')
				parse_fc(ts, str);
			else if (*str == ' ' || *str == '1')
			{
				parse_map(ts, str);
				return ;
			}
			else
			{
				printf("Error\n Settings invalid");
				exit(-1);
			}
		}
	}
}
