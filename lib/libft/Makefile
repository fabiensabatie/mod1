# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/07 17:18:12 by fsabatie          #+#    #+#              #
#    Updated: 2018/03/31 19:45:55 by vlay             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY : all clean fclean re

# Defining variables

NAME = libft.a

#Paths

INCLUDES_PATH = ../includes
IO_PATH = ../io_functions
LIST_PATH = ../list_functions
MEM_PATH = ../memory_functions
STR_PATH =  ../string_functions
VEC_PATH = ../vector_functions
OBJ_PATH = ./build

#Sources files

IO_SRC =	buffer.c ft_printf.c ft_putendl.c ft_putnbr_fd.c ft_putwchar.c get_next_line.c \
			spec_handlers.c ft_atoi.c ft_putchar.c ft_putendl_fd.c ft_putstr.c ft_putwstr.c \
			handlers.c str_printers.c ft_itoa.c ft_putchar_fd.c ft_putnbr.c ft_putstr_fd.c \
			ft_retsplit.c nb_printers.c ft_pos_atoi.c ft_abs.c get_next_word.c

LIST_SRC =	ft_lstadd.c ft_lstdel.c ft_lstdelone.c ft_lstiter.c ft_lstmap.c ft_lstnew.c \
			ft_lstpushback.c ft_lstpop.c ft_lstlink.c

MEM_SRC =	ft_bzero.c ft_memalloc.c ft_memccpy.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memdel.c \
			ft_memmove.c ft_memset.c ft_nrealloc.c

STR_SRC =	ft_chartostr.c ft_isprint.c ft_strcpy.c ft_strjoin.c ft_strncat.c ft_strrchr.c \
			ft_tolower.c ft_chrcstr.c ft_strcat.c ft_strcsub.c ft_strjoin_char.c ft_strncmp.c \
			ft_strrev.c ft_toupper.c ft_chrstr.c ft_strchr.c ft_strdel.c ft_strjoinfree.c \
			ft_strncpy.c ft_strrpos.c ft_wstrlen.c ft_isalnum.c ft_strclr.c ft_strdup.c \
			ft_strlcat.c ft_strnequ.c ft_strsplit.c ft_wstrnsub.c ft_isalpha.c ft_strcmp.c \
			ft_strequ.c ft_strlen.c ft_strnew.c ft_strstr.c ft_isascii.c ft_strcount.c ft_striter.c \
			ft_strmap.c ft_strnstr.c ft_strsub.c ft_isdigit.c ft_strcpos.c ft_striteri.c \
			ft_strmapi.c ft_strnsub.c ft_strtrim.c ft_strpbrk.c ft_strappend.c ft_strndup.c ft_strdupchr.c

VEC_SRC =	ft_vecnew.c ft_vecpush.c

# Prefixing source files with respective paths

IO = $(addprefix $(IO_PATH)/,$(IO_SRC))
LIST = $(addprefix $(LIST_PATH)/,$(LIST_SRC))
MEM = $(addprefix $(MEM_PATH)/,$(MEM_SRC))
STR = $(addprefix $(STR_PATH)/,$(STR_SRC))
VEC = $(addprefix $(VEC_PATH)/,$(VEC_SRC))

# Defining build paths and variables

SOURCES = $(IO) $(LIST) $(MEM) $(STR) $(VEC)
SRCS_LIST = $(IO_SRC) $(LIST_SRC) $(MEM_SRC) $(STR_SRC) $(VEC_SRC)
OBJS_LIST = $(SRCS_LIST:.c=.o)
BUILD_OBJ = $(addprefix $(OBJ_PATH)/,$(OBJS_LIST))

# Compilation flags

CC = gcc
CPPFLAGS = -Wall -Wextra -Werror
INCLUDES = -I $(INCLUDES_PATH)

# Definign rules

all: $(NAME)

$(NAME):
	@mkdir $(OBJ_PATH) && cd $(OBJ_PATH) && $(CC) $(CPPFLAGS) $(INCLUDES) -c $(SOURCES)
	@ar rc $(NAME) $(BUILD_OBJ)
	@ranlib $(NAME)

clean:
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

test: fclean all
	@gcc test.c libft.a -o main
	@./main
