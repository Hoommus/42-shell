/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 14:53:24 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/09 12:08:09 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_history.h"

/*
** This file shares 'history' name with same builtin and builtin __is not here__
** This is done because feature itself is yet another CrUD for history entries.
**
** History builtin is used to read this data and provides user interface.
**
** History is an ArrayList-like collection with expandable array of entries that
** contain the exact user-provided command without any expansions and variable
** resolution.
**
** This collection is saved to a file at HOME in a special format with
** filename of HISTORY_FILE define in twenty_one_sh.h
** File format goes like this (without any leading tabulations):
**     : <unix epoch time>:0;<exact command text>
**     : <unix epoch time>:0;<exact command text
**     with newlines
**     "and any characters;;;">
**     : <unix epoch time>:0;<exact command text>
*/

t_history	*g_history;

void		history_load(int fd)
{
	char		*tmp;
	char		*swap;
	char		*partial;

	history_init_vector(INITIAL_HISTORY_SIZE);
	if (get_next_line(fd, &partial) <= 0)
		return ;
	while (ft_strlen(partial) > 0 && ft_strstr(partial, ":0;") != NULL)
	{
		while (get_next_line(fd, &tmp) > 0 && ft_strstr(tmp, ":0;") == NULL)
		{
			swap = ft_strings_join(2, "\n", partial, tmp);
			chfree_n(2, partial, tmp);
			partial = swap;
		}
		history_push_entry(partial + 15, ft_atoi(partial + 3));
		chfree_n(1, partial);
		partial = tmp;
		tmp = NULL;
	}
	g_history->iterator = g_history->size;
}

void		history_save_entry(int fd)
{
	struct s_history_entry	*last;

	last = g_history->entries[g_history->size - 1];
	g_history->iterator = g_history->size;
	ft_dprintf(fd, ": %10.0ld:0;%s\n", last->timestamp, last->command);
}

char		*history_write(char *command, int history_file)
{
	time_t		timestamp;

	if (!command || ft_strlen(command) == 0 || ft_strlen(command) > 4096)
		return (command);
	time(&timestamp);
	history_push_entry(command, timestamp);
	history_save_entry(history_file);
	return (command);
}

u_int64_t	history_get_size(void)
{
	return (g_history->size);
}
