/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_rules.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 13:37:37 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/03 18:58:26 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_RULES_H
# define SYNTAX_RULES_H

# include <shell_script.h>

/*
# define RULE const t_syntax_rule

# define RULE_PROGRAM				g_program
# define RULE_COMPLETE_COMMANDS		g_complete_commands
# define RULE_COMPLETE_COMMAND		g_complete_command
# define RULE_LIST					g_list
# define RULE_AND_OR				g_and_or
# define RULE_PIPELINE				g_pipeline
# define RULE_PIPE_SEQUENCE			g_pipe_sequence
# define RULE_COMMAND				g_command
# define RULE_COMPOUND_COMMAND		g_compound_command
# define RULE_SUBSHELL				g_subshell
# define RULE_COMPOUND_LIST			g_compound_list
# define RULE_TERM					g_term
# define RULE_FOR_CLAUSE			g_for_clause
# define RULE_NAME					g_name
# define RULE_IN_RULE				g_in
# define RULE_WORDLIST				g_wordlist
# define RULE_IF_CLAUSE				g_if_clause
# define RULE_ELSE_PART				g_else_part
# define RULE_WHILE_CLAUSE			g_while_clause
# define RULE_UNTIL_CLAUSE			g_until_clause
# define RULE_FUNCTION_DEFINITION	g_function_definition
# define RULE_FUNCTION_BODY			g_function_body
# define RULE_FNAME					g_fname
# define RULE_BRACE_GROUP			g_brace_group
# define RULE_DO_GROUP				g_do_group
# define RULE_SIMPLE_COMMAND		g_simple_command
# define RULE_CMD_NAME				g_cmd_name
# define RULE_CMD_WORD				g_cmd_word
# define RULE_CMD_PREFIX			g_cmd_prefix
# define RULE_CMD_SUFFIX			g_cmd_suffix
# define RULE_REDIRECT_LIST			g_redirect_list
# define RULE_IO_REDIRECT			g_io_redirect
# define RULE_IO_FILE				g_io_file
# define RULE_FILENAME				g_filename
# define RULE_IO_HERE				g_io_here
# define RULE_HERE_END				g_here_end
# define RULE_NEWLINE_LIST			g_newline_list
# define RULE_LINEBREAK				g_linebreak
# define RULE_SEPARATOR_OP			g_separator_op
# define RULE_SEPARATOR				g_separator
# define RULE_SEQUENTIAL_SEP		g_sequential_sep

*
** Non-standard grammar rules to correspond to t_rule struct
** and to make mixing of tokens with rules possible
*

# define RULE_SEMICOLON				g_semicolon
# ifdef newline
#  undef newline
# endif
# define RULE_NEWLINE				g_newline
# define RULE_AND_IF				g_and_if
# define RULE_OR_IF					g_or_if

# define RULE_SEQUENTIAL_SEP		t_rule sequential_sep
*/

typedef struct s_syntax_rule	t_rule;

extern const t_rule	g_program;
extern const t_rule	g_complete_commands;
extern const t_rule	g_complete_command;
extern const t_rule	g_list;
extern const t_rule	g_and_or;
extern const t_rule	g_pipeline;
extern const t_rule	g_pipe_sequence;
extern const t_rule	g_command;
extern const t_rule	g_compound_command;
extern const t_rule	g_subshell;
extern const t_rule	g_compound_list;
extern const t_rule	g_term_rule;
extern const t_rule	g_for_clause;
extern const t_rule	g_name;
extern const t_rule	g_in;
extern const t_rule	g_wordlist;
extern const t_rule	g_if_clause;
extern const t_rule	g_else_part;
extern const t_rule	g_while_clause;
extern const t_rule	g_until_clause;
extern const t_rule	g_function_definition;
extern const t_rule	g_function_body;
extern const t_rule	g_fname;
extern const t_rule	g_brace_group;
extern const t_rule	g_do_group;
extern const t_rule	g_simple_command;
extern const t_rule	g_cmd_name;
extern const t_rule	g_cmd_word;
extern const t_rule	g_cmd_prefix;
extern const t_rule	g_cmd_suffix;
extern const t_rule	g_redirect_list;
extern const t_rule	g_io_redirect;
extern const t_rule	g_io_file;
extern const t_rule	g_filename;
extern const t_rule	g_io_here;
extern const t_rule	g_here_end;
extern const t_rule	g_newline_list;
extern const t_rule	g_linebreak;
extern const t_rule	g_separator_op;
extern const t_rule	g_separator;
extern const t_rule	g_sequential_sep;

extern const t_rule	g_semicolon_token;
extern const t_rule	g_newline_token;
extern const t_rule	g_and_if_token;
extern const t_rule	g_or_if_token;
extern const t_rule	g_bang_token;
extern const t_rule	g_pipe_token;
extern const t_rule	g_empty_token;
extern const t_rule	g_lbracket_token;
extern const t_rule	g_rbracket_token;
extern const t_rule	g_lbrace_token;
extern const t_rule	g_rbrace_token;
extern const t_rule	g_for_token;
extern const t_rule	g_word_token;
extern const t_rule	g_if_token;
extern const t_rule	g_then_token;
extern const t_rule	g_fi_token;
extern const t_rule	g_elif_token;
extern const t_rule	g_else_token;
extern const t_rule	g_while_token;
extern const t_rule	g_until_token;
extern const t_rule	g_do_token;
extern const t_rule	g_done_token;
extern const t_rule	g_assignment_word_token;
extern const t_rule	g_io_number_token;
extern const t_rule	g_dless_token;
extern const t_rule	g_dlessdash_token;

extern const t_rule	g_less_token;
extern const t_rule	g_lessand_token;
extern const t_rule	g_great_token;
extern const t_rule	g_greatand_token;
extern const t_rule	g_dgreat_token;
extern const t_rule	g_lessgreat_token;
extern const t_rule	g_clobber_token;

#endif
