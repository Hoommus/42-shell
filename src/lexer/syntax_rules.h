/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_rules.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 13:37:37 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/05 12:30:13 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_RULES_H
# define SYNTAX_RULES_H

# include <shell_script.h>

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

/*
** Function and case rules are omitted. But here they are:
**
** extern const t_rule	g_function_definition;
** extern const t_rule	g_function_body;
** extern const t_rule	g_fname;
** extern const t_rule	g_case_clause;
** extern const t_rule	g_case_list;
** extern const t_rule	g_case_item_ns;
** extern const t_rule	g_case_item;
*/

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