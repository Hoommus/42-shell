/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 12:28:03 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/09 02:40:38 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_history.h"
#include "twenty_one_sh.h"
#include <time.h>

/*
** TODO: Replace reallocation with something more clever
*/

static void	clear_history_hard(void)
{
	extern t_history	*g_history;
	u_int64_t			i;
	u_int64_t			capacity;
	struct stat			s;

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
	if (stat(HISTORY_FILE, &s) == 0)
	{
		ftruncate(get_history_fd(), s.st_size);
		lseek(get_history_fd(), 0, SEEK_SET);
	}
}

/*
** Left place for changed entry '*' mark in that printf
*/

int			hs_history(const char **args)
{
	char					c;
	int						width;
	unsigned long long			i;
	struct s_history_entry	*entry;

	if ((c = flag_validate_short((const char **)args, "cs")))
	{
		ft_dprintf(2, "history: invalid argument -%c\nUsage:"
				"history -c\n", c);
		return (0);
	}
	if (flag_short_present((const char **)args, 'c'))
		clear_history_hard();
	width = ft_nbrlen((u_int64_t)history_get_size()) + 1;
	width = width >= 4 ? width : 4;
	i = 1;
	while ((entry = history_get_entry(i - 1)))
		ft_printf("%*llu%1c %s\n", width, i++, ' ', entry->command);
	return (0);
}
