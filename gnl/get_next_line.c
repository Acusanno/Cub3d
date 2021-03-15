/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 09:03:22 by acusanno          #+#    #+#             */
/*   Updated: 2020/12/17 13:24:26 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*after_lb(char *str, int *ret)
{
	int		i;
	char	*str_return;
	int		ret2;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!(str[i]))
	{
		free(str);
		return (NULL);
	}
	str_return = ft_substr(str, i + 1, (ft_strlen(str) - i), &ret2);
	if (ret2 == -1)
	{
		*ret = -1;
		return (NULL);
	}
	free(str);
	return (str_return);
}

char	*before_lb(char *str, int *ret)
{
	int		i;
	char	*str_return;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	if (!(str_return = malloc(sizeof(char) * (i + 1))))
	{
		*ret = -1;
		return (NULL);
	}
	return (ft_strlcpy(str_return, str, i + 1));
}

int		buff_change(char **buffer, char ***line, char **tmp)
{
	int		ret;

	ret = 0;
	free(*tmp);
	if (*buffer != NULL)
	{
		**line = before_lb(*buffer, &ret);
		*buffer = after_lb(*buffer, &ret);
		if (ret == -1)
			return (-1);
	}
	return (0);
}

int		get_next_line(int fd, char **line)
{
	char			*tmp;
	static char		*buffer;
	int				read_return;

	if (fd < 0 || !line || BUFFER_SIZE <= 0 ||
			(!(tmp = malloc(BUFFER_SIZE + 1))))
		return (-1);
	read_return = 1;
	while (!ft_isline_break(buffer) && read_return != 0)
	{
		if ((read_return = read(fd, tmp, BUFFER_SIZE)) == -1)
			free(tmp);
		if (read_return == -1)
			return (-1);
		tmp[read_return] = '\0';
		if (!(buffer = ft_strjoin(buffer, tmp)))
			return (-1);
	}
	if (buff_change(&buffer, &line, &tmp) == -1)
		return (-1);
	if (read_return == 0)
		free(buffer);
	if (read_return == 0)
		return (0);
	return (1);
}
