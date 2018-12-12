/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_environ.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 18:11:59 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/07 18:11:59 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_ENVIRON_H
# define SHELL_ENVIRON_H

# include <stdbool.h>
# include "libft.h"

enum				e_var_scope
{
	VAR_GLOBAL,
	VAR_LOCAL,
	VAR_EXPORTABLE
};

typedef struct		s_var
{
	char				*key;
	char				*value;
	bool				is_readonly;
	enum e_var_scope	scope;
}					t_var;

extern struct s_var	*g_v_environ;

#endif
