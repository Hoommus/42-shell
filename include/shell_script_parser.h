/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_script_parser.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 13:17:59 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/11 13:24:00 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_SCRIPT_PARSER_H
# define SHELL_SCRIPT_PARSER_H

# include "shell_script.h"
# include "shell_script_syntax.h"

enum							e_parser_state
{
	NONE
};

struct							s_parser_state
{
	t_token				*list_head;
	t_token				*list_tail;
	enum e_token_type	*next_valid_tokens;
};

extern struct s_parser_state	g_parser_state;

#endif
