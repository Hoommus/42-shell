# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/24 10:11:17 by vtarasiu          #+#    #+#              #
#    Updated: 2018/07/01 17:37:26 by vtarasiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

FLAGS = -std=c99 -Wall -Wextra -Werror

HEADER = include
SRC_DIR = ./src/
OBJ_DIR = ./obj/

LIB_DIR = ../libft
LIB_NAME = libft.a

PRINTF_SRC = stringf.c integerf.c nbr_util.c ft_printf.c generic_functions.c \
            generic_functions2.c util.c unicode.c parsing.c evals.c evals2.c \
            variations.c

OBJ = $(addprefix $(OBJ_DIR), $(PRINTF_SRC:.c=.o))

$(NAME): $(OBJ)
	make -C $(LIB_DIR)
	@mkdir -p $(OBJ_DIR)
	cp $(LIB_DIR)/$(LIB_NAME) ./$(NAME)
	@echo "ar rc $(NAME) $(OBJ_DIR)*.o"
	@ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(FLAGS) -I $(HEADER) -o $@ -c $<

all: $(NAME)

debug: all
	gcc -g $(FLAGS) -o $(EXEC) main.c libftprintf.a

clean:
	make -C $(LIB_DIR) clean
	@echo "rm -rf $(OBJ_DIR)*.o"
	@/bin/rm -rf $(OBJ)
	/bin/rm -rf $(OBJ_DIR)

norme:
	norminette $(wildcard $(SRC_DIR)*.c)
	norminette $(HEADER)
	norminette $(wildcard $(LIB_DIR)/*.c)
	norminette $(wildcard $(LIB_DIR)/*.h)

fclean: clean
	make -C $(LIB_DIR) fclean
	/bin/rm -f $(NAME)

re: fclean all

check:
	gcc -c ft_*.c -std=c99 -Wall -Wextra -Werror && norminette ft_*.c *.h

love:
	@echo "Not all."

.PHONY: clean all fclean re check love debug norme