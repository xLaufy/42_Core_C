# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/17 20:28:59 by mkrawczy          #+#    #+#              #
#    Updated: 2024/04/17 20:54:59 by mkrawczy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_printf.a

SRCS = ft_printf.c ft_utils.c ft_hex.c ft_putunbr.c ft_ptr.c

INCS = ft_printf.c

OBJS = $(SRCS:.c=.o)

CC = clang -Wall -Wextra -Werror

RM = rm -f

AR = ar -rcs $@ $<

%.o : %.c
		$(CC) -g -c $< -o ${<:.c=.o}

$(NAME): $(OBJS)
				$(AR) $(NAME) $(OBJS)

all: $(NAME)

clean:
		$(RM) $(OBJS)

fclean: clean
				$(RM) $(NAME)
re: fclean all

.PHONY: all clean fclean re
