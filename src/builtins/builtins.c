/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/25 16:51:36 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_builtins.h"

struct s_builtin	g_builtins[] = {
	{"alias", &hs_alias},
	{"cd", &hs_cd},
	{"echo", &hs_echo},
	{"env", &hs_env},
	{"setenv", &hs_setenv},
	{"unsetenv", &hs_unsetenv},
	{"export", &hs_export},
	{"help", &hs_help},
	{"exit", &hs_exit},
	{"quit", &hs_exit},
	{"where", &hs_where},
	{"history", &hs_history},
	{"tokenizer", &hs_tokenizer},
	{"syntax", &hs_syntax},
	{"set", &hs_set},
	{NULL, NULL}
};

int					hs_echo(char **args)
{
	char	*str;
	char	*swap;

	if (args == NULL || args[0] == NULL)
	{
		ft_printf("\n");
		return (0);
	}
	str = ft_strarr_join(" ", args);
	swap = replace_variables(str);
	chfree(str);
	if (swap == NULL)
		return (1);
	ft_printf("%s\n", swap);
	chfree(swap);
	return (0);
}

int					hs_alias(char **args)
{
	*args = args[0];
	return (0);
}

int					hs_help(char **args)
{
	int		i;

	*args = args[0];
	ft_printf("help: prints all existing builtins\n42sh/2 builtins:\n");
	i = 0;
	while (g_builtins[i + 1].name != NULL)
		ft_printf("%s, ", g_builtins[i++].name);
	ft_printf("%s\n", g_builtins[i].name);
	return (0);
}

int					hs_exit(char **args)
{
	*args = args[0];
	TERM_APPLY_CONFIG(g_term->context_original->term_config);
	exit(0);
}
