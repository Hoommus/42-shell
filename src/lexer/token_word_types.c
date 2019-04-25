/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word_types.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:35:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/02 13:25:32 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

static bool			is_name(const char *str)
{
	if (!str)
		return (false);
	if (!ft_isdigit(str[0]))
		while (*(++str))
			if (!ft_isalnum(*str) && *str != '_')
				return (false);
	return (true);
}

bool				is_io_number(const char *str)
{
	if (!str || (ft_strchr_any(str, "<>") == NULL))
		return (false);
	while (*str)
	{
		if (!ft_isdigit(*str) && *str != '>' && *str != '<')
			return (false);
		str++;
	}
	return (true);
}

static bool			is_assignment_word(const char *str)
{
	char	**split;
	bool	status;

	if (!str || !ft_strchr(str, '='))
		return (false);
	status = false;
	split = ft_strsplit(str, '=');
	if (*str == '=' || *split == NULL || split[1] == NULL || split[2] != NULL)
		status = false;
	else if (is_name(split[0]))
		status = true;
	free_array((void **)split);
	return (status);
}

enum e_token_type	token_class_contextual(const char *str,
											enum e_token_type prev)
{
	enum e_token_type	type;
	int					i;

	i = TOKEN_IF;
	type = TOKEN_NOT_APPLICABLE;
	while (str && g_tokens[i].token_name && type == TOKEN_NOT_APPLICABLE)
	{
		if (ft_strcmp(g_tokens[i].text, str) == 0)
			type = g_tokens[i].type;
		i++;
	}
	if (type == TOKEN_NOT_APPLICABLE)
	{
		if ((prev == TOKEN_ASSIGNMENT_WORD || prev == TOKEN_NEWLINE ||
			prev == TOKEN_SEMICOLON) && is_assignment_word(str))
			type = TOKEN_ASSIGNMENT_WORD;
		else
			type = TOKEN_WORD;
	}
	return (type);
}
