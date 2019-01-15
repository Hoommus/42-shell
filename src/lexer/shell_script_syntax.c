/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_script_syntax.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/03 16:28:37 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/22 17:20:41 by vtarasiu         ###   ########.fr       */
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
//#pragma ide diagnostic ignored "cppcoreguidelines-interfaces-global-init"

const t_rule g_program = { TOKEN,
	.expands_to = {
		{&g_linebreak, &g_complete_commands, &g_linebreak},
		{&g_linebreak}
	},
	"program"
};

/*
** const t_rule g_complete_commands = { TOKEN,
** 	.expands_to = {
** 		{&g_complete_commands, &g_newline_list, &g_complete_command},
** 		{&g_complete_command}
** 	},
** 	"complete_commands"
** };
*/

const t_rule g_complete_commands = {
	TOKEN,
	.expands_to = {
		{&g_complete_command, &g_complete_commands_dash},
	},
	"complete_commands_alt"
};

const t_rule g_complete_commands_dash = {
	TOKEN,
	.expands_to = {
		{&g_complete_command, &g_complete_commands_dash},
		{&g_empty_token},
	},
	"complete_commands_dash"
};

const t_rule g_complete_command = { TOKEN,
	.expands_to = {
		{&g_list, &g_separator_op},
		{&g_list}
	},
	"complete_command"
};

/*
** const t_rule g_list = { TOKEN,
** 	.expands_to = {
** 		{&g_list, &g_separator_op, &g_and_or},
** 		{&g_and_or}
** 	},
** 	"list"
** };
*/

const t_rule g_list = {
	TOKEN,
	.expands_to = {
		{&g_and_or, &g_list_dash}
	},
	"list_alt"
};

const t_rule g_list_dash = {
	TOKEN,
	.expands_to = {
		{&g_separator_op, &g_and_or, &g_list_dash},
		{&g_empty_token}
	},
	"list_dash"
};

/*
** const t_rule g_and_or = {
** 	.token = TOKEN,
** 	.expands_to = {
** 		{&g_pipeline},
** 		{&g_and_or, &g_and_if_token, &g_linebreak, &g_pipeline},
** 		{&g_and_or, &g_or_if_token, &g_linebreak, &g_pipeline}
** 	},
** 	.human_readable = "and_or"
** };
*/

const t_rule g_and_or = {
	.token = TOKEN,
	.expands_to = {
		{&g_pipeline, &g_and_or_dash}
	},
	.human_readable = "and_or_alt"
};

const t_rule g_and_or_dash = {
	.token = TOKEN,
	.expands_to = {
		{&g_and_if_token, &g_linebreak, &g_and_or_dash},
		{&g_or_if_token, &g_linebreak,  &g_and_or_dash}
	},
	.human_readable = "and_or_dash"
};

const t_rule g_pipeline = { TOKEN,
	.expands_to = {
		{&g_pipe_sequence},
		{&g_bang_token, &g_pipe_sequence}
	},
	"pipeline"
};

/*
** const t_rule g_pipe_sequence = { TOKEN,
** 	.expands_to = {
** 		{&g_command},
** 		{&g_pipe_sequence, &g_pipe_token, &g_linebreak, &g_command},
** 	},
** 	"pipe_sequence"
** };
*/

const t_rule g_pipe_sequence = { TOKEN,
	.expands_to = {
		{&g_command, &g_pipe_sequence_dash}
	},
	"pipe_sequence_alt"
};

const t_rule g_pipe_sequence_dash = { TOKEN,
	.expands_to = {
		{&g_pipe_token, &g_linebreak, &g_pipe_sequence_dash},
		{&g_empty_token}
	},
	"pipe_sequence_dash"
};

/*
** The following rule has function_definition removed
*/

const t_rule g_command = { TOKEN,
	.expands_to = {
		{&g_simple_command},
		{&g_compound_command},
		{&g_compound_command, &g_redirect_list}
	},
	"command"
};

/*
** The following rule has case_clause removed.
** This syntax structure is completely removed from this shell
*/

const t_rule g_compound_command = { TOKEN,
	.expands_to = {
		{&g_brace_group},
		{&g_subshell},
		{&g_for_clause},
		{&g_if_clause},
		{&g_while_clause},
		{&g_until_clause},
	},
	"compound_command"
};
const t_rule g_subshell = { TOKEN,
	.expands_to = {
		{&g_lbracket_token, &g_compound_list, &g_rbracket_token}
	},
	"subshell"
};

const t_rule g_compound_list = { TOKEN,
	.expands_to = {
		{&g_linebreak, &g_term_rule},
		{&g_linebreak, &g_term_rule, &g_separator}
	},
	"compound_list"
};

/*
** const t_rule g_term_rule = { TOKEN,
** 	.expands_to = {
** 		{&g_term_rule, &g_separator, &g_and_or},
** 		{&g_and_or}
** 	},
** 	"term_rule"
** };
*/

const t_rule g_term_rule = { TOKEN,
	.expands_to = {
		{&g_and_or, &g_term_rule_dash}
	},
	"term_rule_alt"
};
const t_rule g_term_rule_dash = { TOKEN,
	.expands_to = {
		{&g_separator, &g_and_or, &g_term_rule_dash},
		{&g_empty_token}
	},
	"term_rule_dash"
};

const t_rule g_for_clause = { TOKEN,
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
	},
	"for_clause"
};

const t_rule g_name = { TOKEN_NAME, {{0}}, "name" };
const t_rule g_in = { TOKEN_IN, {{0}}, "in" };

/*
** const t_rule g_wordlist = { TOKEN,
** 	.expands_to = {
** 		{&g_wordlist, &g_word_token},
** 		{&g_word_token}
** 	},
** 	"wordlist"
** };
*/

const t_rule g_wordlist = {
	TOKEN,
	.expands_to = {
		{&g_word_token, &g_wordlist_dash}
	},
	"wordlist_alt"
};
const t_rule g_wordlist_dash = {
	TOKEN,
	.expands_to = {
		{&g_word_token, &g_wordlist_dash},
		{&g_empty_token}
	},
	"wordlist_dash"
};

const t_rule g_if_clause = { TOKEN,
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
	},
	"if_clause"
};
const t_rule g_else_part = { TOKEN,
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
	},
	"else_part"
};
const t_rule g_while_clause = { TOKEN,
	.expands_to = {
		{&g_while_token, &g_compound_list, &g_do_group}
	},
	"while_clause"
};
const t_rule g_until_clause = { TOKEN,
	.expands_to = {
		{&g_until_token, &g_compound_list, &g_do_group}
	},
	"until_clause"
};

/*
** No functions in this shell either, but rules are present, right.
*/

/*
** const t_rule g_function_definition = {TOKEN,
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
** const t_rule g_function_body = { TOKEN,
**     .expands_to = {
**         {&g_complete_command},
**         {&g_complete_command, &g_redirect_list}
**     }
** };
** const t_rule g_fname = { TOKEN,
**     .expands_to = {
**         {&g_name}
**     }
** };
*/

const t_rule g_brace_group = { TOKEN,
	.expands_to = {
		{&g_lbrace_token, &g_compound_list, &g_rbrace_token}
	},
	"brace_group"
};
const t_rule g_do_group = { TOKEN,
	.expands_to = {
		{&g_do_token, &g_compound_list, &g_done_token}
	},
	"do_group"
};
const t_rule g_simple_command = { TOKEN,
	.expands_to = {
		{&g_cmd_prefix, &g_cmd_word, &g_cmd_suffix},
		{&g_cmd_prefix, &g_cmd_word},
		{&g_cmd_prefix},
		{&g_cmd_name, &g_cmd_suffix},
		{&g_cmd_name}
	},
	"simple_command"
};
const t_rule g_cmd_name = { TOKEN_WORD, {{0}}, "cmd_name" }; /* Apply rule 7a */
const t_rule g_cmd_word = { TOKEN_WORD, {{0}}, "cmd_word" }; /* Apply rule 7b */

/*
** const t_rule g_cmd_prefix = { TOKEN,
** 	.expands_to = {
** 		{&g_io_redirect},
** 		{&g_cmd_prefix, &g_io_redirect},
** 		{&g_assignment_word_token},
** 		{&g_cmd_prefix, &g_assignment_word_token}
** 	},
** 	"cmd_prefix"
** };
*/

const t_rule g_cmd_prefix = {
	TOKEN,
	.expands_to = {
		{&g_io_redirect, &g_cmd_prefix_dash},
		{&g_assignment_word_token, &g_cmd_prefix_dash}
	},
	"cmd_prefix_alt"
};
const t_rule g_cmd_prefix_dash = {
	TOKEN,
	.expands_to = {
		{&g_io_redirect, &g_cmd_prefix_dash},
		{&g_assignment_word_token, &g_cmd_prefix_dash},
		{&g_empty_token}
	},
	"cmd_prefix_dash"
};

/*
** const t_rule g_cmd_suffix = { TOKEN,
** 	.expands_to = {
** 		{&g_io_redirect},
** 		{&g_cmd_suffix, &g_io_redirect},
** 		{&g_word_token},
** 		{&g_cmd_suffix, &g_word_token}
** 	},
** 	"cmd_suffix"
** };
*/

const t_rule g_cmd_suffix = { TOKEN,
	.expands_to = {
		{&g_io_redirect, &g_cmd_suffix_dash},
		{&g_word_token, &g_cmd_suffix_dash},
	},
	"cmd_suffix_alt"
};
const t_rule g_cmd_suffix_dash = { TOKEN,
	.expands_to = {
		{&g_io_redirect, &g_cmd_suffix_dash},
		{&g_word_token, &g_cmd_suffix_dash},
		{&g_empty_token}
	},
	"cmd_suffix_dash"
};

/*
** const t_rule g_redirect_list = { TOKEN,
** 	.expands_to = {
** 		{&g_io_redirect},
** 		{&g_redirect_list, &g_io_redirect}
** 	},
** 	"redirect_list"
** };
*/

const t_rule g_redirect_list = { TOKEN,
	.expands_to = {
		{&g_io_redirect, &g_redirect_list_dash}
	},
	"redirect_list_alt"
};
const t_rule g_redirect_list_dash = { TOKEN,
	.expands_to = {
		{&g_io_redirect, &g_redirect_list_dash},
		{&g_empty_token}
	},
	"redirect_list_dash"
};

const t_rule g_io_redirect = { TOKEN,
	.expands_to = {
		{&g_io_file},
		{&g_io_number_token, &g_io_file},
		{&g_io_here},
		{&g_io_number_token, &g_io_here}
	},
	"io_redirect"
};
const t_rule g_io_file = { TOKEN,
	.expands_to = {
		{&g_less_token, &g_filename},
		{&g_lessand_token, &g_filename},
		{&g_great_token, &g_filename},
		{&g_greatand_token, &g_filename},
		{&g_dgreat_token, &g_filename},
		{&g_lessgreat_token, &g_filename},
		{&g_clobber_token, &g_filename},
		{NULL}
	},
	"io_file"
};
const t_rule g_filename = { TOKEN_WORD, {{0}}, "filename" }; /* Apply rule 2 */
const t_rule g_io_here = { TOKEN,
	.expands_to = {
		{&g_dless_token, &g_here_end},
		{&g_dlessdash_token, &g_here_end}
	},
	"io_here"
};
const t_rule g_here_end = { TOKEN_WORD, {{0}}, "here_end" }; /* Apply rule 3 */

/*
** const t_rule g_newline_list = { TOKEN,
** 	.expands_to = {
** 		{&g_newline_token},
** 		{&g_newline_list, &g_newline_token}
** 	},
** 	"newline_list"
** };
*/

const t_rule g_newline_list = { TOKEN,
	.expands_to = {
		{&g_newline_token, &g_newline_list_dash}
	},
	"newline_list_alt"
};

const t_rule g_newline_list_dash = { TOKEN,
	.expands_to = {
		{&g_newline_token, &g_newline_list_dash},
		{&g_empty_token}
	},
	"newline_list_dash"
};

const t_rule g_linebreak = { TOKEN,
	{
		{&g_newline_list},
		{&g_empty_token}
	},
	"linebreak"
};
const t_rule g_separator_op = { TOKEN,
	{
		{&g_semicolon_token},
		{&g_ampersand_token}
	},
	"separator_op"
};
const t_rule g_separator = { TOKEN,
	{
		{&g_separator_op, &g_linebreak},
		{&g_newline_list}
	},
	"separator"
};
const t_rule g_sequential_sep = { TOKEN,
	{
		{&g_semicolon_token, &g_linebreak},
		{&g_newline_list}
	},
	"sequential_sep"
};

const t_rule g_semicolon_token = { TOKEN_SEMICOLON, {{NULL}}, "semicolon_t" };
const t_rule g_ampersand_token = { TOKEN_AMPERSAND, {{NULL}}, "ampersand_t" };
const t_rule g_newline_token = { TOKEN_NEWLINE, {{NULL}}, "newline_t" };
const t_rule g_and_if_token = { TOKEN_AND_IF, {{0}}, "and_if_t" };
const t_rule g_or_if_token = { TOKEN_OR_IF, {{0}}, "or_if_t" };
const t_rule g_bang_token = { TOKEN_BANG, {{0}}, "bang_t" };
const t_rule g_pipe_token = { TOKEN_PIPE, {{0}}, "pipe_t" };
const t_rule g_empty_token = { TOKEN_EMPTY, {{0}}, "empty_t" };
const t_rule g_lbracket_token = { TOKEN_LBRACKET, {{0}}, "lbracket_t" };
const t_rule g_rbracket_token = { TOKEN_RBRACKET, {{0}}, "rbracket_t" };
const t_rule g_lbrace_token = { TOKEN_LBRACE, {{0}}, "lbrace_t" };
const t_rule g_rbrace_token = { TOKEN_RBRACE, {{0}}, "rbrace_t" };
const t_rule g_word_token = { TOKEN_WORD, {{0}}, "word_t" };
const t_rule g_if_token = { TOKEN_IF, {{0}}, "if_t" };
const t_rule g_then_token = { TOKEN_THEN, {{0}}, "then_t" };
const t_rule g_fi_token = { TOKEN_FI, {{0}}, "fi_t" };
const t_rule g_elif_token = { TOKEN_ELIF, {{0}}, "elif_t" };
const t_rule g_else_token = { TOKEN_ELSE, {{0}}, "else_t" };
const t_rule g_for_token = { TOKEN_FOR, {{0}}, "for_t" };
const t_rule g_while_token = { TOKEN_WHILE, {{0}}, "while_t" };
const t_rule g_until_token = { TOKEN_UNTIL, {{0}}, "until_t" };
const t_rule g_do_token = { TOKEN_DO, {{0}}, "do_t" };
const t_rule g_done_token = { TOKEN_DONE, {{0}}, "done_t" };
const t_rule g_assignment_word_token = { TOKEN_ASSIGNMENT_WORD, {{0}},
												"assingment_word_t" };
const t_rule g_io_number_token = { TOKEN_IO_NUMBER, {{0}}, "io_nbr_t" };
const t_rule g_dless_token = { TOKEN_DLESS, {{0}}, "dless_t" };
const t_rule g_dlessdash_token = { TOKEN_DLESSDASH, {{0}}, "dlessdash_t" };
const t_rule g_less_token = { TOKEN_LESS, {{0}}, "less_t" };
const t_rule g_lessand_token = { TOKEN_LESSAND, {{0}}, "lessand_t" };
const t_rule g_great_token = { TOKEN_GREAT, {{0}}, "great_t" };
const t_rule g_greatand_token = { TOKEN_GREATAND, {{0}}, "greatand_t" };
const t_rule g_dgreat_token = { TOKEN_DGREAT, {{0}}, "dgreat_t" };
const t_rule g_lessgreat_token = { TOKEN_LESSGREAT, {{0}}, "lessgreat_t" };
const t_rule g_clobber_token = { TOKEN_CLOBBER, {{0}}, "clobber_t" };

#pragma clang diagnostic pop
