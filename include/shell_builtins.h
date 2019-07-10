/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builtins.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:56:22 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/10 13:19:12 by vtarasiu         ###   ########.fr       */
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
void					print_var_vector(const t_env_vector *vector,
	u_int32_t scopes);
/*
** Builtins (builtins/ *.c)
*/

int						hs_hash(const char **args);
int						hs_alias(const char **args);
int						hs_unalias(const char **args);
int						rn_cd(const char **argv);
int						hs_echo(const char **args);
int						hs_export(const char **args);
int						hs_unset(const char **args);
int						hs_help(const char **args);
int						hs_exit(const char **args);
int						hs_type(const char **args);
int						hs_history(const char **args);
int						hs_tokenizer(const char **args);
int						hs_syntax(const char **args);
int						hs_jobs(const char **args);
int						hs_fg(const char **args);
int						hs_bg(const char **args);
int						hs_true(const char **args);
int						hs_false(const char **args);

int						hs_set(const char **args);

int						rn_pwd(const char **argv);

/*
** Auxiliary
*/

typedef enum			e_path {
	PATH_OK,
	PATH_NOEXIST,
	PATH_NOTDIR,
	PATH_ISDIR,
	PATH_NOPERM
}						t_path;

char					*make_logic_path(const char *path);
int						is_logic(char **argv[], bool *logic, const char *cmd);
const char				*get_env_var_ad(char *varname);
int						set_env_var_ad(char *varname, char *value);

#endif
