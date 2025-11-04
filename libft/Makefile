# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/29 14:27:48 by kuyu              #+#    #+#              #
#    Updated: 2025/10/23 06:16:13 by kuyu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = $(shell find ./ -iname "*.c")

OBJS = $(SRCS:.c=.o)

all: ${NAME}

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

# Pattern Rule (10.5.1)
# '%' in the target, which is a pattern for matching file names. 
# The substring matches '%' is stem. '%' in a prerequisite is the same for '%' in the target
%.o:%.c libft.h
	${CC} ${CFLAGS} -c $< -o $@
# automatic variables: 
# $@ - the file name of the target of the rule
# $< - the name of the first prerequisite
# $^ - the names of all the prerequisites

clean: 
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
