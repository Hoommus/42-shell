#include "../include/minishell.h"

char	*ft_strinsert_range(char *str, char *insert, size_t start, size_t end)
{
	char	*first;
	char	*second;
	char	*final;

	if (insert == NULL)
		return (str);
	first = ft_strsub(str, 0, start);
	second = ft_strsub(str, end, ft_strlen(str) - end);
	final = ft_strings_join(3, "", first, insert, second);
	free(first);
	free(second);
	return (final);
}

char	*replace_home(char *line)
{
	char	*swap;
	char	*dummy;
	char	*copy;
	ssize_t	i;

	if (line == NULL)
		return (NULL);
	i = -1;
	copy = ft_strdup(line);
	while (copy[++i])
		if ((copy[i] == '~' && SNWH && i == 0)
			|| (i > 0 && copy[i] == '~' && ft_iswhsp(copy[i - 1]) && SNWH))
		{
			dummy = get_env("HOME");
			if (dummy != NULL)
			{
				swap = ft_strinsert_range(line, dummy, i, i + 1);
				i = 0;
				chfree_n(2, copy, dummy);
				line = swap;
				copy = swap;
			}
		}
	return (copy);
}

int		get_variable_end(char *line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (!(ft_isalnum(line[i]) || line[i] == '_' || line[i] == '$'))
			break ;
		i++;
	}
	return (i);
}

char	*replace_variables(char *line)
{
	int		i;
	int		end;
	char	*new;
	char	*var;
	char	*swap;

	i = 0;
	new = ft_strdup(line);
	while (new[i])
	{
		if (new[i] == '$')
		{
			end = get_variable_end(line + i) + i;
			if (!is_valid_var((swap = ft_strsub(line, i + 1, end))))
				continue ;
			var = get_env(swap);
			line = ft_strinsert_range(new, var, i, end);
			free(new);
			new = line;
			chfree_n(2, swap, var);
			i--;
		}
		i++;
	}
	return (new);
}
