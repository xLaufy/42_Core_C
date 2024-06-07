
# Define compiler
CC = gcc -Wall -Wextra -Werror

# Removing the files
RM = rm -f

# Define names of the executables
NAME_CLIENT = client
NAME_SERVER = server


# Define directories
LIB_DIR = lib/libft
FT_PRINTF_DIR = lib/ft_printf
INC_DIR = inc


# Define paths to libraries (-L flag adds a directory to the list of directories to be searched for libraries)
LIBFT = $(LIB_DIR)/libft.a
FT_PRINTF = $(FT_PRINTF_DIR)/libftprintf.a
LIB = $(LIB_DIR) $(FT_PRINTF)
INC = inc/minitalk.h
SERVER = src/server/server.c
CLIENT = src/client/client.c

# Make all
all: $(NAME_CLIENT) $(NAME_SERVER)

$(NAME_SERVER): server.o $(LIBFT) $(FT_PRINTF)
				$(CC) -o $@ $^

$(NAME_CLIENT): client.o $(LIBFT) $(FT_PRINTF)
				$(CC) -o $@ $^


# These rules ensure the libft and ft_printf libraries are compiled by invoking make in their directories.(-C flag specifies the directory in which to run make.)

server.o: $(SERVER) $(INC)
		  $(CC) -c $< -o $@

client.o: $(CLIENT) $(INC)
		  $(CC) -c $< -o $@

$(LIBFT):
		  $(MAKE) -C $(LIB_DIR)

$(FT_PRINTF):
			  $(MAKE) -C $(FT_PRINTF_DIR)

# The clean rule removes the object files and the object file directory.
clean:
		$(MAKE) -C $(LIB_DIR) clean
		$(MAKE) -C $(FT_PRINTF_DIR) clean
		$(RM) server.o client.o

# The fclean rule removes the object files, the libraries, and the executables.
fclean: clean
		$(MAKE) -C $(LIB_DIR) fclean
		$(MAKE) -C $(FT_PRINTF_DIR) fclean
		$(RM) $(NAME_CLIENT)
		$(RM) $(NAME_SERVER)

# The re rule is a shortcut for cleaning and rebuilding the project.
re: fclean all

# The .PHONY rule tells make that the listed targets are not files.
.PHONY: all clean fclean re
