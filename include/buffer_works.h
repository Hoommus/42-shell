/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_works.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 17:42:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/01 16:55:03 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_WORKS_H
# define BUFFER_WORKS_H

# include <stdlib.h>
# include <stdbool.h>
# include "libft.h"

union				u_char
{
	u_int64_t		lng;
	char			arr[8];
};

typedef struct		s_symbol
{
	char			s[5];
	u_int8_t		visual_size;
} __attribute__((packed))					t_symbol;

typedef struct		s_buffer
{
	struct s_symbol	*array;
	u_int64_t		size;
	u_int64_t		capacity;
	u_int64_t		iterator;
}					t_buffer;

void				init_buffer_vector(u_int64_t capacity);

void				buff_clear(u_int64_t from_index);
void				buff_clear_part(u_int64_t from_index, u_int64_t to_index);
int					buff_del_symbol(u_int64_t at_index);

t_symbol			*buff_symbol_at(u_int64_t index);
char				*buff_char_at(u_int64_t index);
int					buff_char_at_equals(u_int64_t index, const char *c);
int					buff_char_at_equals_any(u_int64_t index, const char *c);
int64_t				buff_rchr(const char *c, int64_t i);

char				*buff_get_part(u_int64_t from_index, u_int64_t to_index);
t_symbol			*buff_get_part_s(u_int64_t from_index, u_int64_t to_index);

int					buff_insert_single_at(u_int64_t index, const char *str);
int					buff_insert_symbol_at(u_int64_t index, t_symbol *symbol);
int					buff_insert_symbols_at(u_int64_t index, t_symbol *array);
int					buff_insert_string_at(u_int64_t index, const char *string);
u_int32_t			utf_body_size(char first);

int					pb_cut(u_int64_t from, u_int64_t to);
int					pb_copy(u_int64_t from, u_int64_t to);
int					pb_paste(void);
void				pb_clear(void);

#endif
