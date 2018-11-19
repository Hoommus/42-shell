#ifndef SHELL_ENVIRON_H
# define SHELL_ENVIRON_H

# include "libft.h"

enum				e_var_scope
{
	VAR_GLOBAL,
	VAR_LOCAL,
	VAR_EXPORTABLE
};

typedef struct		s_var
{
	char				*var;
	enum e_var_scope	scope;
}					t_var;

extern struct s_var	*g_v_environ;

#endif
