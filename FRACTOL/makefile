
NAME				=	fractol


CC					=	gcc
CFLAGS				=	-Wall -Werror -Wextra
LIBS      			=	-lXext -lX11 -lm
RM					=	rm -f


LIBFT				=	./lib/libft/libft.a
PRINTF				=	./lib/ft_printf/libftprintf.a
MLX					=	./minilibx/libmlx.a
SRC_DIR				=	src/
OBJ_DIR				=	obj/


SRC					=	colors.c error.c events.c zoom.c fractol.c\
						render.c utils.c init.c 

SRCS				=	$(addprefix $(SRC_DIR), $(SRC))

OBJ 				= 	$(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))


all: 					$(MLX) $(LIBFT) $(PRINTF) $(NAME)

$(NAME): 				$(OBJ) $(MLX) $(LIBFT) $(PRINTF)
						@$(CC) $(CFLAGS) $(OBJ) $(MLX) $(LIBFT) $(PRINTF) $(LIBS) -o $(NAME)

$(LIBFT):
						@make -sC ./lib/libft

$(PRINTF):
						@make -sC ./lib/ft_printf

$(MLX):
						@make -sC ./minilibx

$(OBJ_DIR)%.o:			$(SRC_DIR)%.c 
						@mkdir -p $(@D)
						@$(CC) $(CFLAGS) -c $< -o $@

clean:
						@$(RM) -r $(OBJ_DIR)
						@make clean -C ./minilibx
						@make clean -C ./lib/libft
						@make clean -C ./lib/ft_printf

fclean: 				clean
						@$(RM) $(NAME)
						@$(RM) $(MLX)
						@$(RM) $(LIBFT)
						@$(RM) $(PRINTF)

re: 					fclean all

.PHONY: 				all clean fclean re