#include "../include/minishell.h"

char	*replace_variables(char *line, char **environ)
{
	return (line = environ[0]);
}

