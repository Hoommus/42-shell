/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_history.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 12:28:10 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/17 18:05:24 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_HISTORY_H
# define SHELL_HISTORY_H

# include <stdlib.h>
# include <stdbool.h>
# include <time.h>
# include <fcntl.h>
# include "libft.h"
# include "get_next_line.h"
# include "ft_printf.h"

# define INITIAL_HISTORY_SIZE 256

struct						s_history_entry
{
	char		*command;
	time_t		timestamp;
};

typedef struct				s_history_vector
{
	struct s_history_entry	**entries;
	u_int64_t				size;
	u_int64_t				capacity;
	u_int64_t				iterator;
}							t_history;

/*
** History utilities (features/history.c, features/history_vector.c)
*/

void						history_init_vector(const u_int64_t capacity);
void						history_push_entry(const char *cmd, time_t stamp);
void						history_save_entry(int fd);
void						history_load(int fd);
u_int64_t					history_get_size(void);
struct s_history_entry		*history_pop_entry(void);
struct s_history_entry		*history_get_entry(u_int64_t index);
char						*history_write(char *command, int history_file);

void						chfree_n(int n, ...);

#endif
