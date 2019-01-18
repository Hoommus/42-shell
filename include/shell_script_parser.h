/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_script_parser.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 13:17:59 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/21 19:06:12 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_SCRIPT_PARSER_H
# define SHELL_SCRIPT_PARSER_H

# define IS_TERMINAL(rule) (!((rule)->expands_to[0]) || !((rule)->expands_to[0][0]))

# include "shell_script.h"

enum							e_parser_state
{
	NONE
};

typedef struct					s_syntax_error
{
	short		code;
	char		*text;
}								t_error;

typedef struct					s_parser_state
{
	const struct s_syntax_rule	*rule;
	t_token						*list_head;
	t_token						*list_offset;
	t_token						*list_tail;
	int							depth;
	bool						is_syntax_valid : 1;
	t_node						*pending_tree;
}								t_state;

struct							s_result
{
	t_error		*error;
	bool		expansion_requested;
	bool		fatal;
	int			to_skip;
	bool		valid;
};

extern struct s_parser_state	g_ps;

#endif

/*
if readme ; then ;
echo 'The file "readme.txt" exists.'
else ;
echo 'The file "readme.txt" does not exist.'
fi ;
*/
