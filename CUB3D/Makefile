NAME	= cub3d
BIN 	= bin
SRC		= src/main.c src/image.c src/textures.c src/raycaster.c src/parse_id.c \
		 src/parse_map.c src/utils.c src/colission.c src/raycaster_helpers.c \
		 src/raycaster_addons.c src/raycaster_addons2.c src/key_press.c src/key_press2.c\
		 src/parse_map2.c src/raycaster_values.c src/parse_map3.c src/parse_map4.c
INCS	= include/
INCS	= include/
LIBMLX_INCS	= minilibx-linux/
LIBFT	= libft
LIBMLX  = minilibx-linux/
CFLAGS	= -Wall -Werror -Wextra -g
LFLAGS	= -L$(LIBFT) -lft -L$(LIBMLX) -lmlx -lm
IFLAGS	= -I$(INCS) -I$(LIBMLX_INCS)
UNAME	:= $(shell uname)
RM		= rm -rf
OBJS	= $(SRC:src/%c=$(BIN)/%o)
CC		= gcc
FLAGS	= -Lminilibx-linux -lmlx_Linux -lX11 -lXext -g

all: $(NAME)

$(NAME): ${BIN} ${OBJS} | ${LIBFT}
	${CC} ${OBJS} ${FLAGS} ${LFLAGS} -o ${NAME}

$(BIN)/%o: src/%c
	${CC} -c $< ${FLAGS} ${IFLAGS} ${LFLAGS} -o $@

$(BIN):
	mkdir -p $(BIN)

$(LIBFT):
	@make all -C $(LIBFT)

clean:
	$(RM) $(BIN)

fclean: clean
	$(RM) $(NAME)
	@make fclean -C $(LIBFT)

re: fclean all

test:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(NAME)

.PHONY: $(LIBFT) re all clean fclean
