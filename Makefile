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

NAME = 21sh

##### Remove the -g flag #####
FLAGS = -g -std=c99 -Wall -Wextra -Werror

HEADER = include
SRC_DIR = ./src/
OBJ_DIR = ./obj/

LIB_DIR = ./printf
LIB_NAME = libftprintf.a

SHELL_SRC = main.c environ_utils.c commands_execution.c memory.c auxilia.c     \
            signals.c variables_replacement.c \

LEXER_DIR = lexer/
LEXER_SRC = quotes.c smart_split.c tokenizer.c tokens_mem.c

BUILTIN_DIR = builtins/
BUILTIN_SRC = cd.c where.c builtins.c builtins2.c

INTERFACE_DIR = line_editing/
INTERFACE_SRC = buffer_works.c cursor_control.c key_handlers.c listeners.c

OBJ = $(addprefix $(OBJ_DIR), $(SHELL_SRC:.c=.o)) \
      $(addprefix $(OBJ_DIR)$(LEXER_DIR), $(LEXER_SRC:.c=.o)) \
      $(addprefix $(OBJ_DIR)$(BUILTIN_DIR), $(BUILTIN_SRC:.c=.o)) \
      $(addprefix $(OBJ_DIR)$(INTERFACE_DIR), $(INTERFACE_SRC:.c=.o))

all:
	@mkdir -p $(OBJ_DIR)$(LEXER_DIR)
	@mkdir -p $(OBJ_DIR)$(BUILTIN_DIR)
	@mkdir -p $(OBJ_DIR)$(INTERFACE_DIR)
	@make $(NAME)

$(NAME): $(OBJ)
	make -C $(LIB_DIR)
	cp $(LIB_DIR)/$(LIB_NAME) ./$(LIB_NAME)
	gcc $(FLAGS) -o $(NAME) $(OBJ) -I $(HEADER) $(LIB_NAME) -ltermcap

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	gcc $(FLAGS) -I $(HEADER) -o $@ -c $<

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