# Define names of the executables
NAME_CLIENT = client
NAME_SERVER = server

# Define directories
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = ./lib/libft
FT_PRINTF_DIR = ./lib/ft_printf
INC_DIR = inc

# Define paths to source files
SRC_CLIENT = $(SRC_DIR)/client/client.c
SRC_SERVER = $(SRC_DIR)/server/server.c

# Define paths to object files
OBJ_CLIENT = $(SRC_CLIENT:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_SERVER = $(SRC_SERVER:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Define paths to libraries (-L flag adds a directory to the list of directories to be searched for libraries)
LIBFT = $(LIBFT_DIR)/libft.a
FT_PRINTF = $(FT_PRINTF_DIR)/libftprintf.a
LIB = -L$(LIB_DIR) -lft -L$(FT_PRINTF) -lftprintf

# Define paths to headers (-I flag adds a directory to the list of directories to be searched for header files)
HEADERS = -I$(INC_DIR) -I$(LIB_DIR) -I$(FT_PRINTF_DIR)

# Define compiler
CC = gcc -Wall -Wextra -Werror

# Removing the files
RM = rm -f

# Make all
all: $(NAME_CLIENT) $(NAME_SERVER)

# Specifies how to build the client executable from its object file.(-o flag specifies the output file name.)
$(NAME_CLIENT): $(OBJ_CLIENT)
				$(CC) $(OBJ_CLIENT) $(LIB) $(HEADERS) -o $(NAME_CLIENT)

$(NAME_SERVER): $(OBJ_SERVER)
				$(CC) $(OBJ_SERVER) $(LIB) $(HEADERS) -o $(NAME_SERVER)

# The pattern rule for building object files from source files. It ensures the object file directory exists and compiles the source file.
# (/%.o is the target pattern, and /%.c is the prerequisite pattern.) 
# (-p flag creates the directory if it does not exist.)
# (-c $< -o $@ compiles the source file $<  and writes the object file to the object file directory $@.)
# $(@D) is the directory part of the target (obj/client in this scenario), and $(@F) is the file part of the target.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/minitalk.h
				@mkdir -p $(@D)
				$(CC) $(HEADERS) -c $< -o $@

# These rules ensure the libft and ft_printf libraries are compiled by invoking make in their directories.(-C flag specifies the directory in which to run make.)
$(LIBFT):
				$(MAKE) -C $(LIB_DIR)

$(FT_PRINTF_DIR):
				$(MAKE) -C $(FT_PRINTF_DIR)

# The clean rule removes the object files and the object file directory.
clean:
		$(MAKE) -C $(LIB_DIR) clean
		$(MAKE) -C $(FT_PRINTF_DIR) clean
		$(RM) $(OBJ_DIR)

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