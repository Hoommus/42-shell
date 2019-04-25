/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/10 17:14:33 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_builtins.h"

struct s_builtin	g_builtins[] = {
	{"cd", &hs_cd},
	{"echo", &hs_echo},
	{"env", &hs_env},
	{"export", &hs_export},
	{"exit", &hs_exit},
	{"history", &hs_history},
	{"help", &hs_help},
	{"jobs", &hs_jobs},
	{"quit", &hs_exit},
	{"set", &hs_set},
	{"setenv", &hs_setenv},
	{"tokenizer", &hs_tokenizer},
	{"unsetenv", &hs_unsetenv},
	{"where", &hs_where},
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
	return (0);
}

int					hs_help(const char **args)
{
	int		i;

	ft_printf("help: prints all existing builtins\n42sh/2 builtins:\n");
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
	exit(0);
	*args = args[0];
}
