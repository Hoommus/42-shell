/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_script_syntax.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/03 16:28:37 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/10 13:22:30 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script_syntax.h"

/*
** *****************************************************************************
** Below follow grammar rules as described in IEEE Std 1003.1-2017 (§2.10)
** http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html
** These later will be used to analyze syntax of token stream and then pass it
** to parser
** *****************************************************************************
*/

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-interfaces-global-init"

const t_rule g_program = { {0},
	.expands_to = {
		{&g_linebreak, &g_complete_commands, &g_linebreak},
		{&g_linebreak}
	}
};

const t_rule g_complete_commands = {{0},
	.expands_to = {
		{&g_complete_commands, &g_newline_list, &g_complete_command},
		{&g_complete_command}
	}
};

const t_rule g_complete_command = { {0},
	.expands_to = {
		{&g_list, &g_separator_op},
		{&g_list}
	}
};

const t_rule g_list = { {0},
	.expands_to = {
		{&g_list, &g_separator_op, &g_and_or},
		{&g_and_or},
	}
};

const t_rule g_and_or = { {0},
	.expands_to = {
		{&g_pipeline},
		{&g_and_or, &g_and_if_token, &g_linebreak, &g_pipeline},
		{&g_and_or, &g_or_if_token, &g_linebreak, &g_pipeline}
	}
};

const t_rule g_pipeline = { {0},
	.expands_to = {
		{&g_pipe_sequence},
		{&g_bang_token, &g_pipe_sequence}
	}
};

const t_rule g_pipe_sequence = { {0},
	.expands_to = {
		{&g_command},
		{&g_pipe_sequence, &g_pipe_token, &g_linebreak, &g_command},
	}
};

/*
** The following rule has function_definition removed
*/

const t_rule g_command = { {0},
	.expands_to = {
		{&g_simple_command},
		{&g_compound_command},
		{&g_compound_command, &g_redirect_list},
		{NULL}
	}
};

/*
** The following rule has case_clause removed.
** This syntax structure is completely removed from this shell
*/

const t_rule g_compound_command = { {0},
	.expands_to = {
		{&g_brace_group},
		{&g_subshell},
		{&g_for_clause},
		{&g_if_clause},
		{&g_while_clause},
		{&g_until_clause},
	}
};
const t_rule g_subshell = { {0},
	.expands_to = {
		{&g_lbracket_token, &g_compound_list, &g_rbracket_token}
	}
};

const t_rule g_compound_list = { {0},
	.expands_to = {
		{&g_linebreak, &g_term_rule},
		{&g_linebreak, &g_term_rule, &g_separator}
	}
};

const t_rule g_term_rule = { {0},
	.expands_to = {
		{&g_term_rule, &g_separator, &g_and_or},
		{&g_and_or}
	}
};

const t_rule g_for_clause = { {0},
	.expands_to = {
		{
			&g_for_token,
			&g_name,
			&g_do_group
		},
		{
			&g_for_token,
			&g_name,
			&g_sequential_sep,
			&g_do_group
		},
		{
			&g_for_token,
			&g_name,
			&g_linebreak,
			&g_in,
			&g_sequential_sep,
			&g_do_group
		},
		{
			&g_for_token,
			&g_name,
			&g_linebreak,
			&g_in,
			&g_wordlist,
			&g_sequential_sep,
			&g_do_group
		}
	}
};

const t_rule g_name = { {TOKEN_NAME}, {{0}} };
const t_rule g_in = { {TOKEN_IN}, {{0}} };
const t_rule g_wordlist = { {0},
	.expands_to = {
		{&g_wordlist, &g_word_token},
		{&g_word_token}
	}
};
const t_rule g_if_clause = { {0},
	.expands_to = {
		{
			&g_if_token,
			&g_compound_list,
			&g_then_token,
			&g_compound_list,
			&g_else_part,
			&g_fi_token
		},
		{
			&g_if_token,
			&g_compound_list,
			&g_then_token,
			&g_compound_list,
			&g_fi_token
		}
	}
};
const t_rule g_else_part = { {0},
	.expands_to = {
		{
			&g_elif_token,
			&g_compound_list,
			&g_then_token,
			&g_compound_list
		},
		{
			&g_elif_token,
			&g_compound_list,
			&g_then_token,
			&g_compound_list,
			&g_else_part
		},
		{
			&g_else_token,
			&g_compound_list
		}
	}
};
const t_rule g_while_clause = { {0},
	.expands_to = {
		{&g_while_token, &g_compound_list, &g_do_group}
	}
};
const t_rule g_until_clause = {{0},
	.expands_to = {
		{&g_until_token, &g_compound_list, &g_do_group}
	}
};

/*
** No functions in this shell either, but rules are present, right.
*/

/*
** const t_rule g_function_definition = {{0},
**     .expands_to = {
**         {
**             &g_fname,
**             &g_lbracket_token,
**             &g_rbracket_token,
**             &g_linebreak,
**             &g_function_body
**         }
**     }
** };
** const t_rule g_function_body = { {0},
**     .expands_to = {
**         {&g_complete_command},
**         {&g_complete_command, &g_redirect_list}
**     }
** };
** const t_rule g_fname = { {0},
**     .expands_to = {
**         {&g_name}
**     }
** };
*/

const t_rule g_brace_group = { {0},
	.expands_to = {
		{&g_lbrace_token, &g_compound_list, &g_rbrace_token}
	}
};
const t_rule g_do_group = { {0},
	.expands_to = {
		{&g_do_token, &g_compound_list, &g_done_token}
	}
};
const t_rule g_simple_command = { {0},
	.expands_to = {
		{&g_cmd_prefix, &g_cmd_word, &g_cmd_suffix},
		{&g_cmd_prefix, &g_cmd_word},
		{&g_cmd_prefix},
		{&g_cmd_name, &g_cmd_suffix},
		{&g_cmd_name}
	}
};
const t_rule g_cmd_name = { {TOKEN_WORD}, {{0}} }; /* Apply rule 7a */
const t_rule g_cmd_word = { {TOKEN_WORD}, {{0}} }; /* Apply rule 7b */
const t_rule g_cmd_prefix = { {0},
	.expands_to = {
		{&g_io_redirect},
		{&g_cmd_prefix, &g_io_redirect},
		{&g_assignment_word_token},
		{&g_cmd_prefix, &g_assignment_word_token}
	}
};
const t_rule g_cmd_suffix = { {0},
	.expands_to = {
		{&g_io_redirect},
		{&g_cmd_suffix, &g_io_redirect},
		{&g_word_token},
		{&g_cmd_suffix, &g_word_token}
	}
};
const t_rule g_redirect_list = { {0},
	.expands_to = {
		{&g_io_redirect},
		{&g_redirect_list, &g_io_redirect}
	}
};
const t_rule g_io_redirect = { {0},
	.expands_to = {
		{&g_io_file},
		{&g_io_number_token, &g_io_file},
		{&g_io_here},
		{&g_io_number_token, &g_io_here}
	}
};
const t_rule g_io_file = { {0},
	.expands_to = {
		{&g_less_token, &g_filename},
		{&g_lessand_token, &g_filename},
		{&g_great_token, &g_filename},
		{&g_greatand_token, &g_filename},
		{&g_dgreat_token, &g_filename},
		{&g_lessgreat_token, &g_filename},
		{&g_clobber_token, &g_filename}
	}
};
const t_rule g_filename = { {TOKEN_WORD}, {{0}} }; /* Apply rule 2 */
const t_rule g_io_here = { {0},
	.expands_to = {
		{&g_dless_token, &g_here_end},
		{&g_dlessdash_token, &g_here_end}
	}
};
const t_rule g_here_end = { {TOKEN_WORD}, {{0}} }; /* Apply rule 3 */

const t_rule g_newline_list = { {0},
	.expands_to = {
		{&g_newline_token},
		{&g_newline_list, &g_newline_token}
	}
};
const t_rule g_linebreak = { {0},
	{
		{&g_newline_list},
		{&g_empty_token}
	}
};
const t_rule g_separator_op = { {TOKEN_SEMICOLON, TOKEN_AMPERSAND}, {{NULL}} };
const t_rule g_separator = { {0},
	{
		{&g_separator_op, &g_linebreak},
		{&g_newline_list}
	}
};
const t_rule g_sequential_sep = { {0},
	{
		{&g_semicolon_token, &g_linebreak},
		{&g_newline_list}
	}
};

const t_rule g_semicolon_token = { {TOKEN_SEMICOLON}, {{NULL}} };
const t_rule g_newline_token = { {TOKEN_NEWLINE}, {{NULL}} };
const t_rule g_and_if_token = { {TOKEN_AND_IF}, {{0}} };
const t_rule g_or_if_token = { {TOKEN_OR_IF}, {{0}} };
const t_rule g_bang_token = { {TOKEN_BANG}, {{0}} };
const t_rule g_pipe_token = { {TOKEN_PIPE}, {{0}} };
const t_rule g_empty_token = { {TOKEN_EMPTY}, {{0}} };
const t_rule g_lbracket_token = { {TOKEN_LBRACKET}, {{0}} };
const t_rule g_rbracket_token = { {TOKEN_RBRACKET}, {{0}} };
const t_rule g_lbrace_token = { {TOKEN_LBRACE}, {{0}} };
const t_rule g_rbrace_token = { {TOKEN_RBRACE}, {{0}} };
const t_rule g_word_token = { {TOKEN_WORD}, {{0}} };
const t_rule g_if_token = { {TOKEN_IF}, {{0}} };
const t_rule g_then_token = { {TOKEN_THEN}, {{0}} };
const t_rule g_fi_token = { {TOKEN_FI}, {{0}} };
const t_rule g_elif_token = { {TOKEN_ELIF}, {{0}} };
const t_rule g_else_token = { {TOKEN_ELSE}, {{0}} };
const t_rule g_for_token = { {TOKEN_FOR}, {{0}} };
const t_rule g_while_token = { {TOKEN_WHILE}, {{0}} };
const t_rule g_until_token = { {TOKEN_UNTIL}, {{0}} };
const t_rule g_do_token = { {TOKEN_DO}, {{0}} };
const t_rule g_done_token = { {TOKEN_DONE}, {{0}} };
const t_rule g_assignment_word_token = { {TOKEN_ASSIGNMENT_WORD}, {{0}} };
const t_rule g_io_number_token = { {TOKEN_IO_NUMBER}, {{0}} };
const t_rule g_dless_token = { {TOKEN_DLESS}, {{0}} };
const t_rule g_dlessdash_token = { {TOKEN_DLESSDASH}, {{0}} };
const t_rule g_less_token = { {TOKEN_LESS}, {{0}} };
const t_rule g_lessand_token = { {TOKEN_LESSAND}, {{0}} };
const t_rule g_great_token = { {TOKEN_GREAT}, {{0}} };
const t_rule g_greatand_token = { {TOKEN_GREATAND}, {{0}} };
const t_rule g_dgreat_token = { {TOKEN_DGREAT}, {{0}} };
const t_rule g_lessgreat_token = { {TOKEN_LESSGREAT}, {{0}} };
const t_rule g_clobber_token = { {TOKEN_CLOBBER}, {{0}} };

#pragma clang diagnostic pop
