# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/24 10:11:17 by vtarasiu          #+#    #+#              #
#    Updated: 2019/07/25 12:28:23 by vtarasiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 42sh

CC = clang-8

FDS = shell(ulimit -n)

FLAGS = -DSH=\"$(NAME)\" \
               -Wall  \
               -Wextra \
               -Werror # -g3  -fsanitize=address

LIBS = -ltermcap -lcurses

HEADERS = -I include/ -I printf/include -I libft/
SRC_DIR = ./src/
OBJ_DIR = ./obj/

LIB_DIR = ./printf
LIB_NAME = libftprintf.a

SHELL_SRC = main.c init.c memory.c auxilia.c       \
            service_routines.c args_parsing.c string_hash.c \
            shell_environ.c shell_environ_tools.c shell_environ_vector.c \
            shell_environ_memory.c \
            syscall_wrappers.c \
            auxiliary_main.c init_hashtable.c init_term.c

LEXER_DIR = lexer/
LEXER_SRC = tokenizer.c tokens_mem.c token_word_types.c \
            tokenizer_preprocess.c

AST_DIR = ast/
AST_SRC = ast_exec_main.c ast_exec_preprocess.c \
          parser.c syntax_rules.c \
          nodes_memory.c nodes_manipulations.c \
          execution.c \
          exec_command.c exec_command_alterators.c exec_command_heredocs.c \
          exec_command_glob.c exec_subshell.c exec_brace_group.c \
          exec_pipeline.c exec_or_if.c exec_and_if.c \
          tree_auxillary.c \
          tree_simple_command.c tree_simple_command_rdrs.c tree_subshell.c \
          tree_pipe_sequence.c tree_and_or.c tree_list.c \
          tree_brace_group.c

BUILTINS_DIR = builtins/
BUILTINS_SRC = cd.c builtins.c hs_history.c tokenizer_test.c \
               syntax_test.c hs_set.c hs_unset.c \
               hs_export.c hs_jobs.c hs_fg.c hs_bg.c hs_true.c hs_type.c \
               hs_false.c hs_alias.c hs_unalias.c hs_exit.c hs_hash.c \
               rn_cd.c rn_cd_adapter.c rn_cd_is_logic.c rn_cd_logic.c rn_pwd.c

INTERFACE_DIR = line_editing/
INTERFACE_SRC = buffer_drawing.c buffer_input.c  \
                cursor_control.c cursor_positions.c \
                buffer_vector.c buffer_vector_tools1.c buffer_vector_tools2.c  \
                buffer_vector_insertions.c buffer_vector_parts.c \
                state_toggles.c \
                handlers_arrows.c handlers_editing.c handlers_engine.c \
                handlers_arrows_mods.c handlers_arrows_vertical.c \
                handlers_clipboard.c \
                pasteboard_interface.c \
                auxiliary_buffer.c auxiliary_le.c handlers_tab.c acompl_states.c

JOB_CONTROL_DIR = job_control/
JOB_CONTROL_SRC = signals_basic.c signals_children.c signals_child_blocker.c \
                  context_manipulations.c context_switch.c \
                  jc_subshell_env.c \
                  jc_headquaters.c jc_state_updates.c \
                  jc_queue_forknrun.c jc_job_memory.c jc_job_segments.c \
                  pipeline.c jc_job_launcher.c jc_auxiliary.c

EXPANSIONS_DIR = expansions/
EXPANSIONS_SRC = expander_engine.c expand_escaped.c expand_quotes.c \
                 expand_vars.c

HISTORY_DIR = features/history/
HISTORY_SRC = history.c history_vector.c

GLOB_DIR = features/glob/
GLOB_SRC = plist.c parse_dirs.c asterisk_match.c range_match.c \
           ft_glob.c create_pathv.c sort_paths.c dir_match.c

HASH_DIR = features/hash/
HASH_SRC = hash_add.c hash_free.c hash_get.c hash_init.c \
           hash_trunc.c

AUTOCOMPLETE_DIR = features/autocomplete/
AUTOCOMPLETE_SRC = acompl.c acompl_cmd.c acompl_file.c filter_paths.c \
                   acompl_builtin.c acompl_alias.c acompl_hash.c

OBJ = $(addprefix $(OBJ_DIR), $(SHELL_SRC:.c=.o))                           \
      $(addprefix $(OBJ_DIR)$(AST_DIR), $(AST_SRC:.c=.o))                   \
      $(addprefix $(OBJ_DIR)$(HASH_DIR), $(HASH_SRC:.c=.o))                 \
      $(addprefix $(OBJ_DIR)$(GLOB_DIR), $(GLOB_SRC:.c=.o))                 \
      $(addprefix $(OBJ_DIR)$(LEXER_DIR), $(LEXER_SRC:.c=.o))               \
      $(addprefix $(OBJ_DIR)$(HISTORY_DIR), $(HISTORY_SRC:.c=.o))           \
      $(addprefix $(OBJ_DIR)$(BUILTINS_DIR), $(BUILTINS_SRC:.c=.o))         \
      $(addprefix $(OBJ_DIR)$(INTERFACE_DIR), $(INTERFACE_SRC:.c=.o))       \
      $(addprefix $(OBJ_DIR)$(EXPANSIONS_DIR), $(EXPANSIONS_SRC:.c=.o))     \
      $(addprefix $(OBJ_DIR)$(JOB_CONTROL_DIR), $(JOB_CONTROL_SRC:.c=.o))   \
      $(addprefix $(OBJ_DIR)$(AUTOCOMPLETE_DIR), $(AUTOCOMPLETE_SRC:.c=.o)) \

all: $(NAME) 

$(NAME): prepare $(OBJ)
	make -C $(LIB_DIR)
	cp $(LIB_DIR)/$(LIB_NAME) ./$(LIB_NAME)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(HEADERS) $(LIB_NAME) $(LIBS)

prepare:
	@mkdir -p $(OBJ_DIR)$(AST_DIR)
	@mkdir -p $(OBJ_DIR)$(GLOB_DIR)
	@mkdir -p $(OBJ_DIR)$(HASH_DIR)
	@mkdir -p $(OBJ_DIR)$(LEXER_DIR)
	@mkdir -p $(OBJ_DIR)$(HISTORY_DIR)
	@mkdir -p $(OBJ_DIR)$(BUILTINS_DIR)
	@mkdir -p $(OBJ_DIR)$(INTERFACE_DIR)
	@mkdir -p $(OBJ_DIR)$(EXPANSIONS_DIR)
	@mkdir -p $(OBJ_DIR)$(JOB_CONTROL_DIR)
	@mkdir -p $(OBJ_DIR)$(AUTOCOMPLETE_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(FLAGS) $(HEADERS) -o $@ -c $<

install:
	@if [ grep ~/.brew/bin $PATH 2>/dev/null ] ; \
	then \
	    mkdir -p ~/.brew/bin/   ; \
	    cp $(NAME) ~/.brew/bin/ ; \
	    echo "\n export PATH=\$$PATH:\$$HOME/.brew/bin" >> ~/.zshrc ; \
	    source ~/.zshrc         ; \
	    echo "$(NAME) installed"   ; \
	else \
	    cp $(NAME) ~/.brew/bin/ ; \
	    echo "$(NAME) updated"     ; \
	fi ;

clean:
	make -C libft clean
	@echo "rm -rf $(OBJ_DIR)*.o"
	@/bin/rm -rf $(OBJ)
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	make -C $(LIB_DIR) fclean
	/bin/rm -f $(NAME)
	/bin/rm -f $(LIB_NAME)

re: fclean all

love:
	@echo "Not all."


.NOTPARALLEL: prepare
.PHONY: clean all fclean re love

