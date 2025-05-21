CC = cc
<<<<<<< HEAD
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread
=======
CFLAGS = -Wall -Wextra -Werror
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
NAME = philo
SRC = main.c utils.c initialization.c check_arguments.c death.c
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