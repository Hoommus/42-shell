/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_preprocess.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 17:25:02 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 17:26:02 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

/*
** Only moves characters around, no memory reallocation bullshit.
*/

char	*strip_escaped_nl_and_comments(char *string)
{
	register size_t	i;
	size_t			length;

	if (!string)
		return (string);
	length = ft_strlen(string);
	i = 0;
	while (i < length)
	{
		if (string[i] == '#')
			while (string[i] && string[i] != '\n')
				string[i++] = ' ';
		i++;
	}
	i = 0;
	while (length && i < length - 1)
		if (string[i] == '\\' && string[i + 1] == '\n')
			ft_memmove(string + i, string + i + 2, length - i - 1);
		else
			i++;
	return (string);
}
