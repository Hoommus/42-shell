/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_replacement.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/16 14:59:59 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

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
	char	*copy;

	if (line == NULL)
		return (NULL);
	copy = ft_strdup(line);
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

// TODO: rewrite this routine
char	*replace_variables(char *line)
{
	char	*new;

	new = ft_strdup(line);
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
