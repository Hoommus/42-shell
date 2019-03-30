
#include "twenty_one_sh.h"
#include "shell_builtins.h"

int		hs_export(char **args)
{
	if (args == NULL || *args == NULL)
		return (hs_env(args));
	else
		return (hs_setenv(args));
}
