# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/24 10:11:17 by vtarasiu          #+#    #+#              #
#    Updated: 2018/07/19 17:11:28 by vtarasiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

FLAGS = -std=c99 -Wall -Wextra -Werror

HEADER = include
SRC_DIR = ./src/
OBJ_DIR = ./obj/

LIB_DIR = ./printf
LIB_NAME = libftprintf.a

SHELL_SRC = main.c environ_utils.c commands_execution.c builtins.c builtins2.c \
            memory.c auxilia.c input_parsing.c \

OBJ = $(addprefix $(OBJ_DIR), $(SHELL_SRC:.c=.o))

$(NAME): $(OBJ)
	make -C $(LIB_DIR)
	@mkdir -p $(OBJ_DIR)
	cp $(LIB_DIR)/$(LIB_NAME) ./$(LIB_NAME)
	gcc $(FLAGS) -o $(NAME) $(OBJ) -I $(HEADER) $(LIB_NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(FLAGS) -I $(HEADER) -o $@ -c $<

all: $(NAME)

clean:
	make -C libft clean
	@echo "rm -rf $(OBJ_DIR)*.o"
	@/bin/rm -rf $(OBJ)
	/bin/rm -rf $(OBJ_DIR)

norme:
	@echo "<<<===========>>>"
	norminette $(wildcard $(SRC_DIR)*.c)
	@echo "<<<===========>>>"
	norminette $(HEADER)
	@echo "<<<===========>>>"
	norminette $(wildcard $(LIB_DIR)/src/*.c)
	@echo "<<<===========>>>"
	norminette $(wildcard $(LIB_DIR)/include/*.h)
	@echo "<<<===========>>>"
	norminette $(wildcard ./libft/*.c)
	@echo "<<<===========>>>"
	norminette $(wildcard ./libft/*.h)

fclean: clean
	make -C $(LIB_DIR) fclean
	/bin/rm -f $(NAME)
	/bin/rm -f $(LIB_NAME)

re: fclean all

love:
	@echo "Not all."

.PHONY: clean all fclean re love norme