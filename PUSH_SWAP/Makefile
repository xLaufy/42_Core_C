
NAME				= push_swap

LIBFT				= ./lib/libft/libft.a
PRINTF				= ./lib/ft_printf/libftprintf.a
INC					= inc/
SRC_DIR				= src/

CC					= gcc -Wall -Werror -Wextra -I
RM					= rm -f

SRCS 				= $(SRC_DIR)*.c

start:
					@make all
$(LIBFT):
					@make -C ./lib/libft
$(PRINTF):
					@make -C ./lib/ft_printf

all: 				$(NAME)

$(NAME): 			$(LIBFT) $(PRINTF)
					$(CC) $(INC) $(SRCS) $(LIBFT) $(PRINTF) -o $(NAME)


clean:
					@$(RM) -rf $(SRC_DIR)*.o
					@make clean -C ./lib/libft
					@make clean -C ./lib/ft_printf

fclean: 			clean
					@$(RM) $(NAME)
					@$(RM) $(LIBFT)
					@$(RM) $(PRINTF)

re: 				fclean all

.PHONY: 			start all clean fclean re
