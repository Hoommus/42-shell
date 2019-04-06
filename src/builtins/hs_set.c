/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_set.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 17:11:49 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/02 17:09:51 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_history.h"
#include "shell_builtins.h"
#include "twenty_one_sh.h"

void	print_var(const t_var *var)
{
	const char	*spec = "\t\a \r;\n";
	char		*output;

	if (!ft_strchr_any(var->key, spec) && !ft_strchr_any(var->value, spec)
		&& ft_strlen(var->value))
		output = "%s=%s\n";
	else if ((!ft_strchr_any(var->key, spec) && ft_strchr_any(var->value, spec))
		|| !var->value || ft_strlen(var->value) == 0)
		output = "%s='%s'\n";
	else if (ft_strchr_any(var->key, spec) && !ft_strchr_any(var->value, spec))
		output = "\"%s\"=%s\n";
	else
		output = "\"%s\"='%s'\n";
	ft_printf(output, var->key, var->value);
}

int		hs_set(char **args)
{
	const t_var *const	vars = (t_var *)g_term->context_current->environ->array;
	struct s_fd_lst		*swap;
	u_int32_t			i;

	if (args == NULL || *args == NULL)
	{
		i = -1;
		while (++i < g_term->context_current->environ->size)
			if (vars[i].scope & SCOPE_SHELL_LOCAL)
				print_var(vars + i);
		i = -1;
		while (++i < g_term->context_current->environ->size)
			if (!(vars[i].scope & SCOPE_SHELL_LOCAL))
				print_var(vars + i);
		swap = g_term->context_current->fd_list;
		while (swap)
		{
			ft_printf("label: %s o:%d; c:%d\n",
					  swap->label,
					  swap->original,
					  swap->current);
			swap = swap->next;
		}
	}
	else
		ft_dprintf(2, "set: illegal argument(s)\n");
	return (0);
}