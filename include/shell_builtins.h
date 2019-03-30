/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builtins.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:56:22 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/15 15:56:22 by vtarasiu         ###   ########.fr       */
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
	int		(*function) (char **);
};

void					print_var(const t_var *var);

/*
** Builtins (builtins/ *.c)
*/

int						hs_alias(char **args);
int						hs_cd(char **args);
int						hs_echo(char **args);
int						hs_export(char **args);
int						hs_env(char **args);
int						hs_setenv(char **args);
int						hs_unsetenv(char **args);
int						hs_help(char **args);
int						hs_exit(char **args);
int						hs_where(char **args);
int						hs_history(char **args);
int						hs_tokenizer(char **args);
int						hs_syntax(char **args);
int						hs_test(char **args);

int						hs_set(char **args);


#endif
