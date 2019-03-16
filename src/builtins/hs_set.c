/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_set.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 17:11:49 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/16 13:54:51 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_history.h"
#include "shell_builtins.h"
#include "twenty_one_sh.h"

int		hs_set(char **args)
{
	const t_var *const	vars = (t_var *)g_term->context_current->environ->array;
	struct s_fd_lst		*swap;
	u_int32_t			i;

	i = 0;
	while (i < g_term->context_current->environ->size)
	{
		ft_printf("%s = %s\n",
			vars[i].key,
			vars[i].value);
		i++;
	}
	swap = g_term->context_current->fd_list;
	while (swap)
	{
		ft_printf("label: %s o:%d; c:%d\n",
			swap->label,
			swap->original,
			swap->current);
		swap = swap->next;
	}
	*args = args[0];
	return (0);
}
