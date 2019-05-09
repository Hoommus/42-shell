/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 12:28:03 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/10 18:41:13 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_history.h"
#include "twenty_one_sh.h"
#include <time.h>

int			deal_with_flags(const char **args)
{
	return ((int)args);
}

static void	add_entry(const char **args)
{
	char	*whole_command;

	while (*args[0] == '-')
		args++;
	whole_command = ft_strarr_join(" ", (char **)args);
	history_write(whole_command, get_history_fd());
}

/*
** TODO: Replace reallocation with something more clever
*/

static void	clear_history_hard(void)
{
	extern t_history	*g_history;
	u_int64_t			i;
	u_int64_t			capacity;

	capacity = g_history->capacity;
	i = 0;
	while (i < g_history->size)
	{
		free(g_history->entries[i]->command);
		free(g_history->entries[i]);
		i++;
	}
	ft_bzero(g_history, sizeof(t_history));
	free(g_history->entries);
	free(g_history);
	history_init_vector(capacity);
	close_wrapper(init_fd_at_home(HISTORY_FILE, O_TRUNC));
}

/*
** Left place for changed entry '*' mark in that printf
*/

int			hs_history(const char **args)
{
	char					c;
	int						width;
	u_int64_t				i;
	struct s_history_entry	*entry;

	if ((c = flag_validate_short((const char **)args, "cs")))
	{
		ft_dprintf(2, "history: invalid argument -%c\nUsage: history -s arg\n"
				"       history -c\n", c);
		return (0);
	}
	if (flag_short_present((const char **)args, 'c'))
		clear_history_hard();
	if (flag_short_present((const char **)args, 's'))
		add_entry(args);
	width = ft_nbrlen((u_int64_t)history_get_size()) + 1;
	width = width >= 4 ? width : 4;
	i = 1;
	while ((entry = history_get_entry(i - 1)))
		ft_printf("%*llu%1c %s\n", width, i++, ' ', entry->command);
	return (0);
}
