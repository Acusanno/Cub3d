# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/29 10:08:03 by acusanno          #+#    #+#              #
#    Updated: 2021/03/16 10:22:19 by acusanno         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

SRCS =		cub3d.c parsing.c parsing_utils.c map_check.c map_size.c

GCCF = gcc -g3 -fsanitize=address -Wall -Werror -Wextra -g

OBJS = ${SRCS:.c=.o}

_END = \033[0m
_DIM = \033[2m
_YELLOW = \033[33m
_GREEN = \033[92m
_RED = \033[91m
_ROSE = \033[95m
_CYAN = \033[96m

%.o : 		%.c
			${GCCF} -c $< -o $@ -Iminilibx

$(NAME) :	${OBJS} libft/libft.a
			${GCCF} minilibx/libmlx.dylib libft/libft.a $(OBJS) -o $(NAME)
			ln -sf minilibx/libmlx.dylib

libft/libft.a :
			$(MAKE) -C libft/ 

all :		${NAME}

clean :
			rm -f ${OBJS}
			rm -f libmlx.dylib
			${MAKE} -C libft/ clean

fclean :	clean
			rm -f ${NAME}
			rm -f libft/libft.a

re :		fclean all


.PHONY :	all clean fclean re ${NAME}
