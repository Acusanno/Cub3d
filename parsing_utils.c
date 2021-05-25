/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 11:03:53 by acusanno          #+#    #+#             */
/*   Updated: 2021/05/25 14:08:06 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	struct_init(t_settings *ts)
{
	ts->save = 0;
	ts->r[0] = 0;
	ts->r[1] = 0;
	ts->no = 0;
	ts->ea = 0;
	ts->so = 0;
	ts->we = 0;
	ts->s = 0;
	ts->f = -1;
	ts->c = -1;
	ts->map = 0;
	ts->spawn = 0;
}

int	check_fc(char **array)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (ft_atoi(array[i]) > 255 || ft_atoi(array[i]) < 0)
		{
			printf("Error\n Wrong color value for floor or ceiling\n");
			exit(-1);
		}
		i++;
	}
	return (0);
}

void	check_tab(char **tab)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (ft_strncmp(ft_itoa(ft_atoi(tab[i])), tab[i],
				ft_strlen(tab[i])) != 0)
		{
			printf("Error\n Wrong color");
			ft_exit(-1, NULL);
		}
		i++;
	}
}

void	put_tab(char **tab, t_settings *ts, char c)
{
	if (c == 'C')
	{
		ts->c = (ft_atoi(tab[0]) * 256 * 256);
		ts->c += (ft_atoi(tab[1]) * 256);
		ts->c += ft_atoi(tab[2]);
	}
	else if (c == 'F')
	{
		ts->f = (ft_atoi(tab[0]) * 256 * 256);
		ts->f += (ft_atoi(tab[1]) * 256);
		ts->f += ft_atoi(tab[2]);
	}
	check_tab(tab);
}

void	map_realloc(t_settings *ts, char *str, int i)
{
	char	**tmp;
	int		j;

	j = i;
	tmp = ft_calloc(i + 2, sizeof(char *));
	tmp[i] = ft_strdup(str);
	tmp[i + 1] = 0;
	while (j-- > 0)
		tmp[j] = ts->map[j];
	free(ts->map);
	ts->map = tmp;
}
