# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/25 07:14:43 by fsabatie          #+#    #+#              #
#    Updated: 2018/03/31 19:50:30 by vlay             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Defining colors

GREEN = \033[0;32m
RESET = \033[0m

NAME = mod1
CC = gcc

SRC_PATH = ./src/
OBJ_PATH = ./obj/
LIB_PATH = ./lib/
INC_PATH = ./include/ $(LIB_PATH)libft/includes/ $(LIB_PATH)glfw/include/

GCC_FLGS = -Werror -Wextra -Wall
GCC_LIBS = -lglfw3 -framework AppKit -framework OpenGL -framework IOKit -framework CoreVideo

SRC_NAME = mod1.c env.c render.c shaders.c surface.c parser.c

OBJ_NAME = $(SRC_NAME:.c=.o)
LIB_NAME = libft glfw/src

SRC = $(addprefix $(SRC_PATH), $(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))
INC = $(addprefix -I,$(INC_PATH))
LIB = $(addprefix -L$(LIB_PATH),$(LIB_NAME))

all: $(NAME)

$(NAME): $(OBJ)
	@make -C $(LIB_PATH)libft
	@echo "$(GREEN)Compiled libft.$(RESET)"
	@$(CC) $(GCC_FLGS) $(LIB) -lft $(INC) $(OBJ) $(GCC_LIBS) -o $(NAME)
	@echo "$(GREEN)Compiled the binary.$(RESET)"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(GCC_FLGS) $(INC) -o $@ -c $<

clean:
	@rm -f $(OBJ)
	@rm -rf $(OBJ_PATH)

fclean: clean
	@make -C $(LIB_PATH)libft fclean
	@rm -f $(NAME)

re: fclean all
