# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/24 10:11:17 by vtarasiu          #+#    #+#              #
#    Updated: 2019/04/16 11:59:03 by vtarasiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libft.a
OBJ_DIR = ./obj/

CC=clang

FLAGS = -Wall -Wextra -Werror

SRC = ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
      ft_isprint.c ft_itoa.c ft_lstadd.c ft_lstdel.c ft_lstdelone.c            \
      ft_lstiter.c ft_lstmap.c ft_lstnew.c ft_memalloc.c ft_memccpy.c          \
      ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memdel.c ft_memmove.c ft_memset.c \
      ft_nbrlen.c ft_nbrlen_base.c ft_putchar.c ft_putchar_fd.c ft_putendl.c   \
      ft_putendl_fd.c ft_putnbr.c ft_putnbr_fd.c ft_putstr.c ft_putstr_fd.c    \
      ft_strcat.c ft_strchr.c ft_strclr.c ft_strcmp.c ft_strcpy.c ft_strdel.c  \
      ft_strdup.c ft_strequ.c ft_striter.c ft_striteri.c ft_strjoin.c          \
      ft_strlcat.c ft_strlen.c ft_strmap.c ft_strmapi.c ft_strncat.c           \
      ft_strncmp.c ft_strncpy.c ft_strnequ.c ft_strnew.c ft_strnstr.c          \
      ft_strrchr.c ft_strsplit.c ft_strstr.c ft_strsub.c ft_strtrim.c          \
      ft_strulen.c ft_tolower.c ft_toupper.c ft_utf_strlen.c ft_wchar_strlen.c \
      get_next_line.c ft_strsplit_wht.c ft_strings_join.c ft_iswhsp.c          \
      ft_unicode.c ft_rand.c ft_linecount.c strinsert.c

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

$(NAME): $(OBJ)
	@echo "ar rc $(NAME) $(OBJ_DIR)*.o"
	@ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -o $@ -c $<

all: $(NAME)

clean:
	@echo "rm -rf $(OBJ_DIR)*.o"
	@/bin/rm -rf $(OBJ)
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

check:
	$(CC) -c ft_*.c -std=c99 -Wall -Wextra -Werror && norminette ft_*.c *.h

love:
	@echo "Not war."

.PHONY: clean all fclean re check love
