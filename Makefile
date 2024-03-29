# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/29 10:08:03 by acusanno          #+#    #+#              #
#    Updated: 2021/05/31 10:37:16 by acusanno         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

SRCS =		init.c  \
			main.c   \
			cub3d.c   \
			texture.c  \
			parsing.c   \
			display.c    \
			map_size.c    \
			keyboard.c     \
			movement.c      \
			map_check.c      \
			structure.c       \
			wall_rays.c        \
			mlx_utils.c         \
			intersection.c       \
			parsing_utils.c       \
			display_utils.c        \
			parsing_utils2.c        \
			map_check_utils.c        \
			intersection_utils.c

GCCF = gcc -g3 -O3 -g

OBJS = ${SRCS:.c=.o}

_END = \033[0m
_DIM = \033[2m
_YELLOW = \033[33m
_GREEN = \033[92m
_RED = \033[91m
_ROSE = \033[95m
_CYAN = \033[96m
.PHONY :	all clean fclean re libft_

%.o : 		%.c ./cub3d.h
			${GCCF} -c $< -o $@ -Iminilibx 

all :		libft_ ${NAME}

$(NAME) :	${OBJS} ./cub3d.h | libft/libft.a
			${GCCF} minilibx/libmlx.dylib libft/libft.a $(OBJS) -o $(NAME)
			ln -sf minilibx/libmlx.dylib

libft_ :
			$(MAKE) -C libft

libft/libft.a :
			$(MAKE) -C libft

clean :
			rm -f ${OBJS}
			${MAKE} -C libft/ clean

fclean :	clean
			rm -f ${NAME}
			rm -f libmlx.dylib
			${MAKE} -C libft/ fclean

re :		fclean all


