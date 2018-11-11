/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_works.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 17:42:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/10/18 14:22:40 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_WORKS_H
# define BUFFER_WORKS_H

# include <stdlib.h>
# include "libft.h"

typedef struct		s_symbol
{
	char			s[9];
	u_int8_t		visual_size;
}					t_symbol;

typedef struct		s_buffer
{
	struct s_symbol	*array;
	u_int64_t		size;
	u_int64_t		capacity;
	u_int64_t		iterator;
}					t_buffer;

void				init_buffer_vector(u_int64_t capacity);
void				reset_buffer(u_int64_t from_index);
int					buff_del_symbol(u_int64_t index);
char				*buff_get_part(u_int64_t start, u_int64_t end);
t_symbol			*buff_symbol_at(u_int64_t index);
char				*buff_char_at(u_int64_t index);
int					buff_char_at_equals(const u_int64_t index, const char *c);

int					insert_string_at(u_int64_t index, const char *str);
int					insert_symbol_at(u_int64_t index, t_symbol *symbol);

#endif
