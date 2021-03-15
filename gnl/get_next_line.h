/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 12:58:20 by acusanno          #+#    #+#             */
/*   Updated: 2020/12/17 12:48:40 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef _GET_NEXT_LINE_H
# define _GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

int					get_next_line(int fd, char **line);
int					ft_strlen(const char *str);
int					ft_isline_break(const char *s);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strlcpy(char *dst, const char *src, size_t dstsize);
char				*ft_substr(char const *s, int start, size_t len, int *ret2);
char				*after_lb(char *str, int *ret);
char				*before_lb(char *str, int *ret);
#endif
