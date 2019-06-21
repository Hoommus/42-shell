/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/21 12:34:06 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "twenty_one_sh.h"
#include "shell_builtins.h"

extern bool			g_is_subshell_env;

struct s_builtin	g_builtins[] = {
	{"cd", &hs_cd},
	{"echo", &hs_echo},
	{"export", &hs_export},
	{"exit", &hs_exit},
	{"history", &hs_history},
	{"help", &hs_help},
	{"jobs", &hs_jobs},
	{"quit", &hs_exit},
	{"set", &hs_set},
	{"tokenizer", &hs_tokenizer},
	{"unset", &hs_unsetenv},
	{"where", &hs_where},
	{"alias", &hs_alias},
	{"unalias", &hs_unalias},
	{NULL, NULL}
};

int					hs_echo(const char **args)
{
	char	*str;

	if (args == NULL || args[0] == NULL)
	{
		ft_printf("\n");
		return (0);
	}
	str = ft_strarr_join(" ", (char **)args);
	ft_printf("%s\n", str);
	free(str);
	return (0);
}

int					hs_help(const char **args)
{
	int		i;

	ft_printf("help: prints all existing builtins\n42sh builtins:\n");
	i = 0;
	while (g_builtins[i + 1].name != NULL)
		ft_printf("%s, ", g_builtins[i++].name);
	ft_printf("%s\n", g_builtins[i].name);
	*args = args[0];
	return (0);
}

int					hs_exit(const char **args)
{
	TERM_APPLY_CONFIG(g_term->context_original->term_config);
	if (args && *args)
	{
		if (!is_string_numeric(*args, 10))
		{
			ft_dprintf(2, SH ": exit: %s: numeric argument required\n", *args);
			if (g_is_subshell_env)
				return (2);
			else
				exit(2);
		}
		else if (g_is_subshell_env)
			return (ft_atoi(*args));
		else if (jc_get()->active_jobs != NULL)
			return ((ft_dprintf(2, "You have running jobs.\n") & 0) | 1);
		else
			exit(ft_atoi(*args));
	}
	if (g_is_subshell_env)
		return (0);
	else if (jc_get()->active_jobs != NULL)
		return ((ft_dprintf(2, "You have running jobs.\n") & 0) | 1);
	else
		exit(0);
}
