/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_vector.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 15:35:35 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/09 15:58:37 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_history.h"

extern t_history		*g_history;

void					history_init_vector(const u_int64_t capacity)
{
	g_history = (t_history *)ft_memalloc(sizeof(t_history));
	g_history->capacity = capacity;
	g_history->size = 0;
	g_history->iterator = 0;
	g_history->entries = (struct s_history_entry **)
			ft_memalloc(capacity * sizeof(struct s_history_entry *));
}

static void				history_reallocate_vector(void)
{
	struct s_history_entry	**tmp;
	u_int64_t				sz;

	g_history->capacity += g_history->capacity / 2;
	tmp = (struct s_history_entry **)
			ft_memalloc(sizeof(struct s_history_entry *) * g_history->capacity);
	sz = sizeof(struct s_history_entry *) * g_history->size;
	ft_memcpy(tmp, g_history->entries, sz);
	free(g_history->entries);
	g_history->entries = tmp;
}

bool					is_string_empty(const char *str)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(str);
	i = 0;
	while (i < len)
	{
		if (ft_strchr(LIBFT_WHTSP, str[i]) == NULL)
			return (false);
		i++;
	}
	return (true);
}

void					history_push_entry(const char *cmd, time_t timestamp)
{
	struct s_history_entry	*entry;

	if (!cmd || is_string_empty(cmd))
		return ;
	if (g_history->size >= g_history->capacity - 1)
		history_reallocate_vector();
	entry = (struct s_history_entry *)
			ft_memalloc(sizeof(struct s_history_entry));
	entry->command = ft_strdup(cmd);
	entry->timestamp = timestamp;
	g_history->entries[(g_history->size)++] = entry;
}

struct s_history_entry	*history_get_entry(u_int64_t index)
{
	if (index >= g_history->size)
		return (NULL);
	return (g_history->entries[index]);
}
