# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/24 10:11:17 by vtarasiu          #+#    #+#              #
#    Updated: 2018/10/30 19:27:48 by vtarasiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh

##### Remove the -g flag #####
FLAGS = -g -std=c99 -Wall -Wextra -Werror

HEADER = -I include/ -I printf/include -I libft/
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
INTERFACE_SRC = buffer_drawing.c buffer_works.c     \
                cursor_control.c cursor_positions.c \
                buffer_vector.c buffer_vector_tools1.c buffer_vector_tools2.c  \
                state_machine.c \
                handlers_arrows.c handlers_editing.c handlers_zbase.c \

HISTORY_DIR = features/history/
HISTORY_SRC = history.c history_vector.c

OBJ = $(addprefix $(OBJ_DIR), $(SHELL_SRC:.c=.o))                 \
      $(addprefix $(OBJ_DIR)$(LEXER_DIR), $(LEXER_SRC:.c=.o))     \
      $(addprefix $(OBJ_DIR)$(BUILTIN_DIR), $(BUILTIN_SRC:.c=.o)) \
      $(addprefix $(OBJ_DIR)$(HISTORY_DIR), $(HISTORY_SRC:.c=.o)) \
      $(addprefix $(OBJ_DIR)$(INTERFACE_DIR), $(INTERFACE_SRC:.c=.o))

all: $(NAME)

#                     ** Do not EVER touch rule below **                       #
#                     ** Do not EVER touch rule below **                       #
#                     ** Do not EVER touch rule below **                       #
$(NAME): prepare $(OBJ)
	@BUILD_NBR=$$(expr $$(grep -E "# define BUILD [0-9]+" \
			   < ./include/twenty_one_sh.h | \
			   grep -o -E '[0-9]+') + 1) &&  \
	BUILD_DATE=$$(date +"%d.%m.%y %T %Z") && \
	ex -c "%s/define BUILD [0-9]\+/define BUILD $$BUILD_NBR/g|             \
			%s!define BUILD_DATE .\+!define BUILD_DATE \"$$BUILD_DATE\"!g| \
			|w|q" include/twenty_one_sh.h
	rm -f obj/main.o
	gcc $(FLAGS) $(HEADER) -o $(OBJ_DIR)main.o -c $(SRC_DIR)main.c
	make -C $(LIB_DIR)
	cp $(LIB_DIR)/$(LIB_NAME) ./$(LIB_NAME)
	gcc $(FLAGS) -o $(NAME) $(OBJ) $(HEADER) $(LIB_NAME) -ltermcap

prepare:
	@mkdir -p $(OBJ_DIR)$(LEXER_DIR)
	@mkdir -p $(OBJ_DIR)$(BUILTIN_DIR)
	@mkdir -p $(OBJ_DIR)$(HISTORY_DIR)
	@mkdir -p $(OBJ_DIR)$(INTERFACE_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	gcc $(FLAGS) $(HEADER) -o $@ -c $< ;

install: all
	@if [ grep ~/.brew/bin $PATH 2>/dev/null ] ; \
	then \
	    mkdir -p ~/.brew/bin/   ; \
	    cp $(NAME) ~/.brew/bin/ ; \
	    echo "\n export PATH=\$$PATH:\$$HOME/.brew/bin" >> ~/.zshrc ; \
	    source ~/.zshrc         ; \
	    echo "21sh installed"   ; \
	else \
	    cp $(NAME) ~/.brew/bin/ ; \
	    echo "21sh updated"     ; \
	fi ;

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

re: fclean $(NAME)

love:
	@echo "Not all."

.PHONY: clean all fclean re love norme
