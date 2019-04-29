# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/24 10:11:17 by vtarasiu          #+#    #+#              #
#    Updated: 2019/04/26 12:32:42 by vtarasiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh

CC = clang

FDS = shell(ulimit -n)

##### Remove the -g flag #####
FLAGS = -g -Wall  \
               -Wextra \
               -Werror  \
               -Wno-unknown-pragmas \
               #-fsanitize="address"

HEADER = -I include/ -I printf/include -I libft/
SRC_DIR = ./src/
OBJ_DIR = ./obj/

LIB_DIR = ./printf
LIB_NAME = libftprintf.a

SHELL_SRC = main.c init.c memory.c auxilia.c       \
            service_routines.c args_parsing.c string_hash.c \
            shell_environ.c shell_environ_tools.c shell_environ_vector.c \
            syscall_wrappers.c

LEXER_DIR = lexer/
LEXER_SRC = smart_split.c tokenizer.c tokens_mem.c token_word_types.c \
            tokenizer_preprocess.c

AST_DIR = ast/
AST_SRC = ast_exec_main.c ast_exec_preprocess.c \
          parser.c syntax_rules.c \
          nodes_memory.c nodes_manipulations.c \
          execution.c \
          exec_command.c exec_command_alterators.c exec_command_heredocs.c \
          exec_subshell.c \
          exec_pipeline.c exec_andor_if.c\
          tree_auxillary.c \
          tree_simple_command.c tree_simple_command_rdrs.c tree_subshell.c \
          tree_pipe_sequence.c tree_and_or.c tree_list.c

BUILTIN_DIR = builtins/
BUILTIN_SRC = cd.c where.c builtins.c hs_history.c tokenizer_test.c \
              syntax_test.c hs_set.c hs_env.c hs_setenv.c hs_unsetenv.c \
              hs_export.c hs_jobs.c

INTERFACE_DIR = line_editing/
INTERFACE_SRC = buffer_drawing.c buffer_input.c  \
                cursor_control.c cursor_positions.c \
                buffer_vector.c buffer_vector_tools1.c buffer_vector_tools2.c  \
                state_toggles.c state_updates.c \
                handlers_arrows.c handlers_editing.c handlers_engine.c \
                handlers_arrows_mods.c handlers_arrows_vertical.c \
                handlers_clipboard.c \
                auxiliary_buffer.c auxiliary_le.c

JOB_CONTROL_DIR = job_control/
JOB_CONTROL_SRC = signals_basic.c \
                  context_manipulations.c context_switch.c command_lookup.c \
                  jc_headquaters.c  \
                  jc_queue_execution.c jc_queue_forknrun.c jc_queue_interface.c

EXPANSIONS_DIR = expansions/
EXPANSIONS_SRC = expander_engine.c expand_escaped.c expand_quotes.c \
                 expand_vars.c

HISTORY_DIR = features/history/
HISTORY_SRC = history.c history_vector.c

OBJ = $(addprefix $(OBJ_DIR), $(SHELL_SRC:.c=.o))                         \
      $(addprefix $(OBJ_DIR)$(AST_DIR), $(AST_SRC:.c=.o))                 \
      $(addprefix $(OBJ_DIR)$(LEXER_DIR), $(LEXER_SRC:.c=.o))             \
      $(addprefix $(OBJ_DIR)$(BUILTIN_DIR), $(BUILTIN_SRC:.c=.o))         \
      $(addprefix $(OBJ_DIR)$(HISTORY_DIR), $(HISTORY_SRC:.c=.o))         \
      $(addprefix $(OBJ_DIR)$(INTERFACE_DIR), $(INTERFACE_SRC:.c=.o))     \
      $(addprefix $(OBJ_DIR)$(EXPANSIONS_DIR), $(EXPANSIONS_SRC:.c=.o))   \
      $(addprefix $(OBJ_DIR)$(JOB_CONTROL_DIR), $(JOB_CONTROL_SRC:.c=.o)) \

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
	$(CC) $(FLAGS) $(HEADER) -o $(OBJ_DIR)main.o -c $(SRC_DIR)main.c
	make -C $(LIB_DIR)
	cp $(LIB_DIR)/$(LIB_NAME) ./$(LIB_NAME)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(HEADER) $(LIB_NAME) -ltermcap

prepare:
	@mkdir -p $(OBJ_DIR)$(AST_DIR)
	@mkdir -p $(OBJ_DIR)$(LEXER_DIR)
	@mkdir -p $(OBJ_DIR)$(BUILTIN_DIR)
	@mkdir -p $(OBJ_DIR)$(HISTORY_DIR)
	@mkdir -p $(OBJ_DIR)$(INTERFACE_DIR)
	@mkdir -p $(OBJ_DIR)$(EXPANSIONS_DIR)
	@mkdir -p $(OBJ_DIR)$(JOB_CONTROL_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(FLAGS) $(HEADER) -o $@ -c $< ;

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
