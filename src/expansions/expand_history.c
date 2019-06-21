/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_history.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 13:51:34 by obamzuro          #+#    #+#             */
/*   Updated: 2019/06/21 18:54:26 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansions_internal.h"
#include "twenty_one_sh.h"
#include "shell_history.h"

extern t_history		*g_history;

static int			strstr_begin(char *str, char *expanded)
{
	if (!str || !expanded || !*str || !*expanded)
		return (0);
	while (*str && *expanded)
	{
		if (!*expanded)
			return (1);
		if (*str != *expanded)
			return (0);
		++str;
		++expanded;
	}
	return (1);
}

static void			expand_double_pang(char *str)
{
	char		*swap;
	char		*previous_command;

	if (!g_history->size)
		previous_command = "";
	else
		previous_command = g_history->entries[g_history->size - 1]->command;
	swap = ft_strjoin(previous_command, str + 2);
	free(str);
	str = swap;
}

static void			expand_bang(char *str)
{
	u_int64_t		i;
	u_int64_t		limit;
	char			*swap;

	if (!str || *str != '!')
		return ;
	if (str[1] == '!')
	{
		expand_double_pang(str);
		return ;
	}
	i = g_history->size - 1;
	limit = i <= HISTORY_EXPANSION_OLDEST ? 0 :
		i - HISTORY_EXPANSION_OLDEST;
	while (i > limit)
	{
		if (strstr_begin(g_history->entries[i - 1]->command, str + 1))
		{
			swap = str;
			str = ft_strdup(g_history->entries[i - 1]->command);
			free(swap);
		}
		--i;
	}
}

char				*expand_history(char *str)
{
	u_int32_t		i;
	char			**words;
	char			*result;

	i = -1;
	words = ft_strsplit_wht(str);
	while (words[++i])
		expand_bang(words[i]);
	result = ft_strarr_join("", words);
	i = -1;
	while (words[++i])
		free(words[i]);
	free(words);
	return (result);
}
