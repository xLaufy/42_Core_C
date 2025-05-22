CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread
NAME = philo
SRC = src/main.c src/simulation.c src/utils.c src/initialization.c src/check_arguments.c src/death.c
OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME) : $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
