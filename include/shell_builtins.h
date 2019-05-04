/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builtins.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:56:22 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/04 15:56:33 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_BUILTINS_H
# define SHELL_BUILTINS_H

# include "libft.h"
# include "get_next_line.h"
# include "ft_printf.h"
# include "shell_environ.h"

struct					s_builtin
{
	char	*name;
	int		(*function) (const char **);
};

void					print_var(const t_var *var);

/*
** Builtins (builtins/ *.c)
*/

int						hs_alias(const char **args);
int						hs_cd(const char **args);
int						hs_echo(const char **args);
int						hs_export(const char **args);
int						hs_env(const char **args);
int						hs_setenv(const char **args);
int						hs_unsetenv(const char **args);
int						hs_help(const char **args);
int						hs_exit(const char **args);
int						hs_where(const char **args);
int						hs_history(const char **args);
int						hs_tokenizer(const char **args);
int						hs_syntax(const char **args);
int						hs_test(const char **args);
int						hs_jobs(const char **args);
int						hs_fg(const char **args);
int						hs_bg(const char **args);

int						hs_set(const char **args);

#endif
