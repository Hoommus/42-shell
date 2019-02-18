/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_rules.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/03 16:28:37 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/18 13:38:14 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script_parser.h"

/*
** *****************************************************************************
** Below follow grammar rules as described in IEEE Std 1003.1-2017 (§2.10)
** http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html
** These later are used to analyze syntax of token stream and build
** corresponding binary syntax tree
** *****************************************************************************
*/

#pragma clang diagnostic push
//#pragma ide diagnostic ignored "cppcoreguidelines-interfaces-global-init"

/*
** const t_rule g_complete_commands = {
** 	.token = TOKEN_NOT_APPLICABLE,
** 	.expands_to = {
** 		{&g_complete_commands, &g_newline_list, &g_complete_command},
** 		{&g_complete_command}
** 	},
** 	"complete_commands"
** };
*/

const t_rule g_complete_command = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_list, &g_separator},
		{&g_list},
		{&g_newline_list}
	},
	.human_readable = "complete_command",
	.tree_builder = NULL
};

/*
** const t_rule g_list = {
** 	.token = TOKEN_NOT_APPLICABLE,
** 	.expands_to = {
** 		{&g_list, &g_separator_op, &g_and_or},
** 		{&g_and_or}
** 	},
** 	"list"
** };
*/

const t_rule g_list = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_and_or, &g_list_dash}
	},
	.human_readable = "list_alt",
	.tree_builder = NULL
};

const t_rule g_list_dash = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_separator, &g_and_or, &g_list_dash},
		{&g_empty_token}
	},
	.human_readable = "list_dash",
	.tree_builder = NULL
};

const t_rule g_and_or = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_pipeline, &g_and_or_dash}
	},
	.human_readable = "and_or_alt",
	.tree_builder = NULL
};

const t_rule g_and_or_dash = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_and_if_token, &g_linebreak, &g_pipeline, &g_and_or_dash},
		{&g_or_if_token, &g_linebreak, &g_pipeline, &g_and_or_dash},
		{&g_empty_token}
	},
	.human_readable = "and_or_dash",
	.tree_builder = NULL
};

const t_rule g_pipeline = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_pipe_sequence},
		{&g_bang_token, &g_pipe_sequence}
	},
	.human_readable = "pipeline",
	.tree_builder = NULL
};

const t_rule g_pipe_sequence = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_command, &g_pipe_sequence_dash}
	},
	.human_readable = "pipe_sequence_alt",
	.tree_builder = NULL
};

const t_rule g_pipe_sequence_dash = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_pipe_token, &g_linebreak, &g_command, &g_pipe_sequence_dash},
		{&g_empty_token}
	},
	.human_readable = "pipe_sequence_dash",
	.tree_builder = NULL
};

/*
** The following rule has function_definition removed
*/

const t_rule g_command = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_simple_command},
		{&g_compound_command, &g_redirect_list},
		{&g_compound_command}
	},
	.human_readable = "command",
	.tree_builder = NULL
};

/*
** The following rule has case_clause removed.
** This syntax structure is completely removed from this shell
*/

const t_rule g_compound_command = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_brace_group},
		{&g_subshell},
		{&g_if_clause},
		{&g_while_clause}
	},
	.human_readable = "compound_command",
	.tree_builder = NULL
};
const t_rule g_subshell = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_lbracket_token, &g_compound_list, &g_rbracket_token}
	},
	.human_readable = "subshell",
	.tree_builder = NULL
};

const t_rule g_compound_list = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_term_rule},
		{&g_newline_list, &g_term_rule},
		{&g_term_rule, &g_separator},
		{&g_newline_list, &g_term_rule, &g_separator}
	},
	.human_readable = "compound_list",
	.tree_builder = NULL
};

/*
** const t_rule g_term_rule = {
** 	.token = TOKEN_NOT_APPLICABLE,
** 	.expands_to = {
** 		{&g_term_rule, &g_separator, &g_and_or},
** 		{&g_and_or}
** 	},
** 	"term_rule"
** };
*/

const t_rule g_term_rule = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_and_or, &g_term_rule_dash}
	},
	.human_readable = "term_rule_alt",
	.tree_builder = NULL
};
const t_rule g_term_rule_dash = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_separator, &g_and_or, &g_term_rule_dash},
		{&g_empty_token}
	},
	.human_readable = "term_rule_dash",
	.tree_builder = NULL
};

/*
** const t_rule g_for_clause = {
** 	.token = TOKEN_NOT_APPLICABLE,
** 	.expands_to = {
** 		{
** 			&g_for_token,
** 			&g_name,
** 			&g_do_group
** 		},
** 		{
** 			&g_for_token,
** 			&g_name,
** 			&g_sequential_sep,
** 			&g_do_group
** 		},
** 		{
** 			&g_for_token,
** 			&g_name,
** 			&g_linebreak,
** 			&g_in,
** 			&g_sequential_sep,
** 			&g_do_group
** 		},
** 		{
** 			&g_for_token,
** 			&g_name,
** 			&g_linebreak,
** 			&g_in,
** 			&g_wordlist,
** 			&g_sequential_sep,
** 			&g_do_group
** 		}
** 	},
** 	.human_readable = "for_clause",
** 	.tree_builder = NULL
** };
*/

/*
** const t_rule g_name = { TOKEN_NAME, {{0}}, "name", NULL };
** const t_rule g_in = { TOKEN_IN, {{0}}, "in", NULL };
*/

/*
** const t_rule g_wordlist = {
**	.token = TOKEN_NOT_APPLICABLE,
** 	.expands_to = {
** 		{&g_wordlist, &g_word_token},
** 		{&g_word_token}
** 	},
** 	"wordlist"
** };
*/

const t_rule g_wordlist = {
	TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_word_token, &g_wordlist_dash}
	},
	.human_readable = "wordlist_alt",
	.tree_builder = NULL
};

const t_rule g_wordlist_dash = {
	TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_word_token, &g_wordlist_dash},
		{&g_empty_token}
	},
	.human_readable = "wordlist_dash",
	.tree_builder = NULL
};

const t_rule g_if_clause = {
	.token = TOKEN_NOT_APPLICABLE,
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
	.human_readable = "if_clause",
	.tree_builder = NULL
};
const t_rule g_else_part = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{
			&g_elif_token,
			&g_simple_command, &g_separator,
			&g_then_token,
			&g_compound_list
		},
		{
			&g_elif_token,
			&g_simple_command, &g_separator,
			&g_then_token,
			&g_compound_list,
			&g_else_part
		},
		{
			&g_else_token,
			&g_compound_list
		}
	},
	.human_readable = "else_part",
	.tree_builder = NULL
};

const t_rule g_while_clause = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_while_token, &g_simple_command, &g_separator, &g_do_group}
	},
	.human_readable = "while_clause",
	.tree_builder = NULL
};

/*
** const t_rule g_until_clause = {
** 	.token = TOKEN_NOT_APPLICABLE,
** 	.expands_to = {
** 		{&g_until_token, &g_compound_list, &g_do_group}
** 	},
** 	.human_readable = "until_clause",
** 	.tree_builder = NULL
** };
*/

/*
** No functions in this shell either, but rules are present, right.
*/

/*
** const t_rule g_function_definition = {TOKEN_NOT_APPLICABLE,
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
** const t_rule g_function_body = {
**  .token = TOKEN_NOT_APPLICABLE,
**     .expands_to = {
**         {&g_complete_command},
**         {&g_complete_command, &g_redirect_list}
**     }
** };
** const t_rule g_fname = {
**  .token = TOKEN_NOT_APPLICABLE,
**     .expands_to = {
**         {&g_name}
**     }
** };
*/

const t_rule g_brace_group = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_lbrace_token, &g_compound_list, &g_rbrace_token}
	},
	.human_readable = "brace_group",
	.tree_builder = NULL
};
const t_rule g_do_group = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_do_token, &g_compound_list, &g_done_token}
	},
	.human_readable = "do_group",
	.tree_builder = NULL
};
const t_rule g_simple_command = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_cmd_prefix, &g_cmd_word, &g_cmd_suffix},
		{&g_cmd_prefix, &g_cmd_word},
		{&g_cmd_prefix},
		{&g_cmd_name, &g_cmd_suffix},
		{&g_cmd_name}
	},
	.human_readable = "simple_command",
	.tree_builder = NULL
};

/*
** Apply rule 7a
*/
const t_rule g_cmd_name = { TOKEN_WORD, {{0}}, "cmd_name", NULL };

/*
** Apply rule 7b
*/
const t_rule g_cmd_word = { TOKEN_WORD, {{0}}, "cmd_word", NULL };

/*
** const t_rule g_cmd_prefix = {
** 	.token = TOKEN_NOT_APPLICABLE,
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
	TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_io_redirect, &g_cmd_prefix_dash},
		{&g_assignment_word_token, &g_cmd_prefix_dash}
	},
	.human_readable = "cmd_prefix_alt",
	.tree_builder = NULL
};
const t_rule g_cmd_prefix_dash = {
	TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_io_redirect, &g_cmd_prefix_dash},
		{&g_assignment_word_token, &g_cmd_prefix_dash},
		{&g_empty_token}
	},
	.human_readable = "cmd_prefix_dash",
	.tree_builder = NULL
};

/*
** const t_rule g_cmd_suffix = {
**	.token = TOKEN_NOT_APPLICABLE,
** 	.expands_to = {
** 		{&g_io_redirect},
** 		{&g_cmd_suffix, &g_io_redirect},
** 		{&g_word_token},
** 		{&g_cmd_suffix, &g_word_token}
** 	},
** 	"cmd_suffix"
** };
*/

const t_rule g_cmd_suffix = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_io_redirect, &g_cmd_suffix_dash},
		{&g_word_token, &g_cmd_suffix_dash},
	},
	.human_readable = "cmd_suffix_alt",
	.tree_builder = NULL
};
const t_rule g_cmd_suffix_dash = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_io_redirect, &g_cmd_suffix_dash},
		{&g_word_token, &g_cmd_suffix_dash},
		{&g_empty_token}
	},
	.human_readable = "cmd_suffix_dash",
	.tree_builder = NULL
};

/*
** const t_rule g_redirect_list = {
**	.token = TOKEN_NOT_APPLICABLE,
** 	.expands_to = {
** 		{&g_io_redirect},
** 		{&g_redirect_list, &g_io_redirect}
** 	},
** 	"redirect_list"
** };
*/

const t_rule g_redirect_list = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_io_redirect, &g_redirect_list_dash}
	},
	.human_readable = "redirect_list_alt",
	.tree_builder = NULL
};
const t_rule g_redirect_list_dash = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_io_redirect, &g_redirect_list_dash},
		{&g_empty_token}
	},
	.human_readable = "redirect_list_dash",
	.tree_builder = NULL
};

const t_rule g_io_redirect = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_io_file},
		{&g_io_number_token, &g_io_file},
		{&g_io_here},
		{&g_io_number_token, &g_io_here},
		{&g_io_number_token}
	},
	.human_readable = "io_redirect",
	.tree_builder = NULL
};
const t_rule g_io_file = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_less_token, &g_filename},
		{&g_lessand_token, &g_filename},
		{&g_great_token, &g_filename},
		{&g_greatand_token, &g_filename},
		{&g_dgreat_token, &g_filename},
		{&g_lessgreat_token, &g_filename},
		{&g_clobber_token, &g_filename}
	},
	.human_readable = "io_file",
	.tree_builder = NULL
};
/*
** Apply rule 2
*/
const t_rule g_filename = { TOKEN_WORD, {{0}}, "filename", NULL };
const t_rule g_io_here = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_dless_token, &g_here_end},
		{&g_dlessdash_token, &g_here_end}
	},
	.human_readable = "io_here",
	.tree_builder = NULL
};

/*
** Apply rule 3
*/
const t_rule g_here_end = { TOKEN_WORD, {{0}}, "here_end", NULL };

/*
** const t_rule g_newline_list = {
**	.token = TOKEN_NOT_APPLICABLE,
** 	.expands_to = {
** 		{&g_newline_token},
** 		{&g_newline_list, &g_newline_token}
** 	},
** 	"newline_list"
** };
*/

const t_rule g_newline_list = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_newline_token, &g_newline_list_dash}
	},
	.human_readable = "newline_list_alt",
	.tree_builder = NULL
};

const t_rule g_newline_list_dash = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_newline_token, &g_newline_list_dash},
		{&g_empty_token}
	},
	.human_readable = "newline_list_dash",
	.tree_builder = NULL
};

const t_rule g_linebreak = {
	.token = TOKEN_NOT_APPLICABLE,
	{
		{&g_newline_list},
		{&g_empty_token}
	},
	.human_readable = "linebreak",
	.tree_builder = NULL
};
const t_rule g_separator_op = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_semicolon_token},
		{&g_ampersand_token}
	},
	.human_readable = "separator_op",
	.tree_builder = NULL
};
const t_rule g_separator = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_separator_op, &g_linebreak},
		{&g_newline_list}
	},
	.human_readable = "separator",
	.tree_builder = NULL
};
const t_rule g_sequential_sep = {
	.token = TOKEN_NOT_APPLICABLE,
	.expands_to = {
		{&g_semicolon_token, &g_linebreak},
		{&g_newline_list}
	},
	.human_readable = "sequential_sep",
	.tree_builder = NULL
};

const t_rule g_semicolon_token = {
	TOKEN_SEMICOLON,
	{{NULL}},
	"semicolon_t",
	.tree_builder = NULL
};
const t_rule g_ampersand_token = {
	TOKEN_AMPERSAND,
	{{0}},
	"ampersand_t",
	.tree_builder = NULL
};
const t_rule g_newline_token = {
	TOKEN_NEWLINE,
	{{0}},
	"newline_t",
	.tree_builder = NULL
};
const t_rule g_and_if_token = {
	TOKEN_AND_IF,
	{{0}},
	"and_if_t",
	.tree_builder = NULL
};
const t_rule g_or_if_token = {
	TOKEN_OR_IF,
	{{0}},
	"or_if_t",
	.tree_builder = NULL
};
const t_rule g_bang_token = {
	TOKEN_BANG,
	{{0}},
	"bang_t",
	.tree_builder = NULL
};
const t_rule g_pipe_token = {
	TOKEN_PIPE,
	{{0}},
	"pipe_t",
	.tree_builder = NULL
};
const t_rule g_empty_token = {
	TOKEN_EMPTY,
	{{0}},
	"empty_t",
	.tree_builder = NULL
};
const t_rule g_lbracket_token = {
	TOKEN_LBRACKET,
	{{0}},
	"lbracket_t",
	.tree_builder = NULL
};
const t_rule g_rbracket_token = {
	TOKEN_RBRACKET,
	{{0}},
	"rbracket_t",
	.tree_builder = NULL
};
const t_rule g_lbrace_token = {
	TOKEN_LBRACE,
	{{0}},
	"lbrace_t",
	.tree_builder = NULL
};
const t_rule g_rbrace_token = {
	TOKEN_RBRACE,
	{{0}},
	"rbrace_t",
	.tree_builder = NULL
};
const t_rule g_word_token = {
	TOKEN_WORD,
	{{0}},
	"word_t",
	.tree_builder = NULL
};
const t_rule g_if_token = {
	TOKEN_IF,
	{{0}},
	"if_t",
	.tree_builder = NULL
};
const t_rule g_then_token = {
	TOKEN_THEN,
	{{0}},
	"then_t",
	.tree_builder = NULL
};
const t_rule g_fi_token = {
	TOKEN_FI,
	{{0}},
	"fi_t",
	.tree_builder = NULL
};
const t_rule g_elif_token = {
	TOKEN_ELIF,
	{{0}},
	"elif_t",
	.tree_builder = NULL
};
const t_rule g_else_token = {
	TOKEN_ELSE,
	{{0}},
	"else_t",
	.tree_builder = NULL
};
/*
** const t_rule g_for_token = {
** 	TOKEN_FOR,
** 	{{0}},
** 	"for_t",
** 	.tree_builder = NULL
** };
*/
const t_rule g_while_token = {
	TOKEN_WHILE,
	{{0}},
	"while_t",
	.tree_builder = NULL
};
/*
**const t_rule g_until_token = {
**	TOKEN_UNTIL,
**	{{0}},
**	"until_t",
**	.tree_builder = NULL
**};
*/
const t_rule g_do_token = {
	TOKEN_DO,
	{{0}},
	"do_t",
	.tree_builder = NULL
};
const t_rule g_done_token = {
	TOKEN_DONE,
	{{0}},
	"done_t",
	.tree_builder = NULL
};
const t_rule g_assignment_word_token = {
	TOKEN_ASSIGNMENT_WORD,
	{{0}},
	"assingment_word_t",
	.tree_builder = NULL
};
const t_rule g_io_number_token = {
	TOKEN_IO_NUMBER,
	{{0}},
	"io_nbr_t",
	.tree_builder = NULL
};
const t_rule g_dless_token = {
	TOKEN_DLESS,
	{{0}},
	"dless_t",
	.tree_builder = NULL
};
const t_rule g_dlessdash_token = {
	TOKEN_DLESSDASH,
	{{0}},
	"dlessdash_t",
	.tree_builder = NULL
};
const t_rule g_less_token = {
	TOKEN_LESS,
	{{0}},
	"less_t",
	.tree_builder = NULL
};
const t_rule g_lessand_token = {
	TOKEN_LESSAND,
	{{0}},
	"lessand_t",
	.tree_builder = NULL
};
const t_rule g_great_token = {
	TOKEN_GREAT,
	{{0}},
	"great_t",
	.tree_builder = NULL
};
const t_rule g_greatand_token = {
	TOKEN_GREATAND,
	{{0}},
	"greatand_t",
	.tree_builder = NULL
};
const t_rule g_dgreat_token = {
	TOKEN_DGREAT,
	{{0}},
	"dgreat_t",
	.tree_builder = NULL
};
const t_rule g_lessgreat_token = {
	TOKEN_LESSGREAT,
	{{0}},
	"lessgreat_t",
	.tree_builder = NULL
};
const t_rule g_clobber_token = {
	TOKEN_CLOBBER,
	{{0}},
	"clobber_t",
	.tree_builder = NULL
};

#pragma clang diagnostic pop
