/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word_types.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:35:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/12 17:16:04 by vtarasiu         ###   ########.fr       */
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

static bool			is_io_number(const char *str)
{
	if (!str && ft_strchr_any(str, "<>") == NULL)
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

	if (!str)
		return (false);
	status = false;
	split = ft_strsplit(str, '=');
	if (*str == '=' || *split == NULL || split[1] == NULL || split[2] != NULL)
		status = false;
	else if (is_name(split[0]))
		status = true;
	free_array(split);
	return (status);
}

enum e_token_type	get_token_type_contextual(const char *str)
{
	enum e_token_type	type;
	enum e_token_type	last_token;
	int					i;

	i = TOKEN_IF;
	type = TOKEN;
	while (str && g_tokens[i].token_name != NULL && type == TOKEN)
	{
		if (ft_strcmp(g_tokens[i].text, str) == 0)
			type = g_tokens[i].type;
		i++;
	}
	last_token = TOKEN_EMPTY;
	if (g_ps.list_tail != NULL)
		last_token = g_ps.list_tail->type;
	if (type == TOKEN)
	{
		if (is_assignment_word(str))
			type = TOKEN_ASSIGNMENT_WORD;
		else if (is_io_number(str))
			type = TOKEN_IO_NUMBER;
//		else if (g_parser_state.list_tail == NULL
//			|| last_token == TOKEN_SEMICOLON
//			|| last_token == TOKEN_NEWLINE)
//			type = TOKEN_WORD_COMMAND;
		else
			type = TOKEN_WORD;
	}
	return (type);
}
