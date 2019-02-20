/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_works.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 17:42:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/16 19:06:24 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_WORKS_H
# define BUFFER_WORKS_H

# include <stdlib.h>
# include <stdbool.h>
# include "libft.h"

/*
** Using union just to use it anywhere
*/

union				u_char
{
	u_int64_t		lng;
	char			arr[8];
};

typedef struct		s_symbol
{
	char			s[9];
	u_int8_t		visual_size : 2;
}					t_symbol;

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
char				*buff_get_part(u_int64_t from_index, u_int64_t to_index);
t_symbol			*buff_symbol_at(u_int64_t index);
char				*buff_char_at(u_int64_t index);
int					buff_char_at_equals(u_int64_t index, const char *c);
int					buff_char_at_equals_any(u_int64_t index, const char *c);
int					buff_chroff(t_buffer *buffer, const char *c, int i);

int					buff_insert_single_at(u_int64_t index, const char *str);
int					buff_insert_symbol_at(u_int64_t index, t_symbol *symbol);
int					buff_insert_string_at(u_int64_t index, const char *string);
u_int64_t			utf_body_size(char first);

#endif
