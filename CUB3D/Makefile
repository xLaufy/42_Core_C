NAME	= cub3d 
BIN 	= bin
SRC		= src/main.c src/image.c src/textures.c src/raycaster.c src/parse_id.c src/parse_map.c src/utils.c
INCS	= include/
LIBMLX_INCS	= /usr/local/include
LIBFT	= libft
LIBMLX  = /usr/local/lib
CFLAGS	= -Wall -Werror -Wextra -g -O3 
LFLAGS	= -L$(LIBFT) -lft -L$(LIBMLX) -lmlx -lm
IFLAGS	= -I$(INCS) -I$(LIBMLX_INCS) 
UNAME	:= $(shell uname)
RM		= rm -rf
OBJS	= $(SRC:src/%c=$(BIN)/%o)
CC		= gcc
FLAGS	= -Lminilibx-linux -lmlx_Linux -lX11 -lXext

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
