/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/08/01 12:32:25 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/twenty_one_sh.h"

char	*ft_strinsert_range(char *str, char *insert, size_t start, size_t end)
{
	char	*first;
	char	*second;
	char	*final;

	if (insert == NULL)
		insert = "";
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

	dummy = get_env("HOME");
	if (line == NULL)
		return (NULL);
	i = -1;
	copy = ft_strdup(line);
	while (dummy && copy[++i])
	{
		if ((copy[i] == '~' && SNWH && i == 0) ||
			(i > 0 && copy[i] == '~' && ft_iswhsp(copy[i - 1]) && SNWH))
		{
			swap = ft_strinsert_range(line, dummy, i, i + 1);
			i = 0;
			chfree(copy);
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

	i = -1;
	new = ft_strdup(line);
	while (new[++i])
	{
		if (new[i] == '$')
		{
			len = get_variable_end(new + i);
			if (len == 0 || !is_valid_var((swap = ft_strsub(new, i + 1, len))))
				continue ;
			var = get_env(swap);
			line = ft_strinsert_range(new, var, i, len + i + 1);
			free(new);
			new = line;
			i = var == NULL ? i : i - 1;
			chfree(swap);
		}
	}
	return (new);
}

void	expand_variables(char **line)
{
	char	*swap;

	swap = replace_home(*line);
	chfree(*line);
	*line = replace_variables(swap);
	chfree(swap);
}
