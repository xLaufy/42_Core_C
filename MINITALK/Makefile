
CLIENT = client
SERVER = server

# Define compiler
CC = gcc -Wall -Wextra -Werror -I


# Removing the files
RM = rm -f


# Define names of the executables
LIBFT = ./lib/libft/libft.a
FT_PRINTF = ./lib/ft_printf/libftprintf.a
INC_DIR = inc/
SRC_DIR = src/
OBJ_DIR = obj/


SRC_CLIENT = $(addprefix $(SRC_DIR), $(addsuffix .c, $(CLIENT)))
SRC_SERVER = $(addprefix $(SRC_DIR), $(addsuffix .c, $(SERVER)))

OBJ_CLIENT = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(CLIENT)))
OBJ_SERVER = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SERVER)))


start:
		@make all


$(LIBFT):
			@make -C ./lib/libft


$(FT_PRINTF):
				@make -C ./lib/ft_printf


all: $(CLIENT) $(SERVER)


$(CLIENT):	$(OBJ_CLIENT) $(LIBFT) $(FT_PRINTF)
						 @$(CC) $(INC_DIR) $(OBJ_CLIENT) $(LIBFT) $(FT_PRINTF) -o $(CLIENT)


$(SERVER):	$(OBJ_SERVER) $(LIBFT) $(FT_PRINTF)
						 @$(CC) $(INC_DIR) $(OBJ_SERVER) $(LIBFT) $(FT_PRINTF) -o $(SERVER)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c
								@mkdir -p $(@D)
								@$(CC) $(INC_DIR) -c $< -o $@

clean:
		@$(RM) -r $(OBJ_DIR) $(LIBFT) $(FT_PRINTF)
		@make clean -C ./lib/libft
		@make clean -C ./lib/ft_printf


fclean: clean
		$(RM) $(CLIENT) $(SERVER)
		$(RM) $(LIB)


re: fclean all

# The .PHONY rule tells make that the listed targets are not files.
.PHONY: all clean fclean re
