/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/21 11:24:58 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "twenty_one_sh.h"
#include "shell_builtins.h"

struct s_builtin	g_builtins[] = {
	{"alias", &hs_alias},
	{"bg", &hs_bg},
	{"cd", &rn_cd},
	{"echo", &hs_echo},
	{"export", &hs_export},
	{"exit", &hs_exit},
	{"fg", &hs_fg},
	{"false", &hs_false},
	{"hash", &hs_hash},
	{"history", &hs_history},
	{"help", &hs_help},
	{"jobs", &hs_jobs},
	{"quit", &hs_exit},
	{"set", &hs_set},
	{"tokenizer", &hs_tokenizer},
	{"true", &hs_true},
	{"type", &hs_type},
	{"unalias", &hs_unalias},
	{"unset", &hs_unset},
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
