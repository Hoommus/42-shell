/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/31 14:45:48 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	while (ft_isalnum(line[i]) || line[i] == '_' || line[i] == '$')
		i++;
	return (i - 1);
}

char	*replace_variables(char *line)
{
	int		i;
	int		len;
	char	*new;
	char	*var;
	char	*swap;

	i = 0;
	new = ft_strdup(line);
	while (new[i])
	{
		if (new[i] == '$')
		{
			len = get_variable_end(new + i);
			if (!is_valid_var((swap = ft_strsub(new, i + 1, len))))
				continue ;
			var = get_env(swap);
			line = ft_strinsert_range(new, var, i, len + i + 1);
			free(new);
			new = line;
			chfree_n(2, swap, var);
			i--;
		}
		i++;
	}
	return (new);
}

void	expand_variables(char **line)
{
	char	*swap;

	swap = replace_home(*line);
	free(*line);
	*line = replace_variables(swap);
	free(swap);
}
