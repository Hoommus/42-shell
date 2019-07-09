/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:19:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/09 03:55:13 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_builtins.h"
#include "shell_job_control.h"

static int		set_tricky(const char *str)
{
	t_env_vector		*vector;
	char				tmp[1024];
	int					j;

	vector = jc_is_subshell() ? g_term->context_current->environ
							: g_term->context_original->environ;
	ft_bzero(tmp, sizeof(char) * 1024);
	j = -1;
	while (str[++j])
		if (j >= 1024)
			return ((ft_dprintf(2, "export: var name is too long\n") & 0) | 1);
		else if ((str[j] == '=' || !str[j])
			&& is_valid_var(ft_memcpy(tmp, str, j)))
			return (!set_env_v(vector, tmp, str + j + 1, SCOPE_EXPORT));
		else if (str[j] == '=' && !is_valid_var(tmp))
		{
			ft_dprintf(2, "export: '%s' is not a valid variable name\n",
						ft_strlen(tmp) == 0 ? str : tmp);
			return (1);
		}
	return (0);
}

static void		set_single(const char *arg)
{
	t_var			*var;
	t_env_vector	*vector;

	vector = jc_is_subshell() ? g_term->context_current->environ
							: g_term->context_original->environ;
	var = environ_get_entry(vector, arg);
	if (var == NULL)
		environ_push_entry(vector, arg, "", SCOPE_EXPORT);
	else
		environ_update_entry(vector, arg, ft_strdup(var->value), SCOPE_EXPORT);
}

int				hs_setenv(const char **args)
{
	int		i;

	i = 0;
	if (args[0] == NULL)
		ft_printf("usage:\nexport [-p] [KEY=VALUE ...]\n");
	while (args[i])
	{
		if (ft_strchr(args[i], '=') == NULL && is_valid_var(args[i]))
			set_single(args[i]);
		else if (set_tricky(args[i]) == 1)
		{
			ft_dprintf(2, "export: arguments is not a KEY=VALUE pair\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int				hs_export(const char **args)
{
	const t_var		*vars = (t_var *)g_term->context_current->environ->array;
	u_int32_t		i;
	bool			is_p;

	if (args == NULL || *args == NULL || (is_p = !ft_strcmp(args[0], "-p")))
	{
		i = 0;
		while (i < g_term->context_current->environ->size)
		{
			if (vars[i].scope & SCOPE_EXPORT)
			{
				if (is_p)
					ft_printf("export ");
				print_var(vars + i);
			}
			i++;
		}
		return (0);
	}
	return (hs_setenv(args));
}
