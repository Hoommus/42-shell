/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_script_parser.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 13:17:59 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/23 16:16:56 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_SCRIPT_PARSER_H
# define SHELL_SCRIPT_PARSER_H

# define INDENT_0 "│ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │"
# define INDENT INDENT_0 INDENT_0 INDENT_0 INDENT_0 INDENT_0 INDENT_0 INDENT_0
# define IS_TERMINAL(rule) (!(rule)->expands_to[0] || !(rule)->expands_to[0][0])

# define LOG_INSIDE "%.*s%s inside [ \x1b[36m%s\x1b[0m ];\n"
# define LOG_TERMINAL "%.*s  %s terminal in rule [%s] at %s (%s) \n"
# define LOG_EMPTY "%.*s    \x1b[35m%s empty allowed, validating\x1b[0m\n"
# define LOG_EXPECT "%.*s    \x1b[32m%s expected: (%s); got (%s)\x1b[0m\n"

# include "shell_script.h"

typedef struct					s_parser_state
{
	const struct s_syntax_rule	*rule;
	t_token						*list_head;
	t_token						*list_offset;
	int							depth;
	bool						logging;
}								t_state;

/*
** Syntax rule is an entity that contains an array of possible expansions
** which define language grammar.
**
** Rules which given rule expands to are stored in 'expands_to' field.
** The expands_to field is a two dimensional NULL-terminated array of pointers
** to s_syntax_rule variables declared in syntax_rules.h.
** Basically, it can hold up to 9 cases of given rule each of which
** can hold up to 9 rules and tokens to which this rule expands.
** For example, this syntax rule from BNF will be translated to this
** structure as follows:
**
** Original:
**  command : simple_command
**          | compound_command
**          | compound_command redirect_list
**          | function_definition
**          ;
**
** This shell struct:
**  t_rule command = {
**      {0}, // because it cannot be any token
**
**      // Token stream must correspond to either of these .expands_to rules
**      // to be valid 'command'
**      .expands_to = {
**          { &simple_command },      // command can be solely simple_command
**          { &compound_command },    // OR compound_command
**          // OR compound_command which is followed by redirect_list
**          { &compound_command, &redirect_list },
**          { &function_definition }, // OR be a function definition
**      },
**      .human_readable = "command",
**      .tree_builder = &command_build
**  };
*/

/*
** Forward declarations
*/
typedef struct s_build_result	t_bresult;
struct s_result;

typedef t_bresult				*(*t_builder) (const t_state *state,
	struct s_result *last_build);

typedef struct					s_syntax_rule
{
	const enum e_token_type					token;
	const struct s_syntax_rule *restrict	expands_to[8][6];
	const char *restrict const				human_readable;
	t_builder								tree_builder;
} __attribute__((packed))								t_rule;

typedef struct					s_syntax_error
{
	short			code;
	char			*text;
}								t_error;

struct							s_build_result
{
	t_node			*root;
	const t_rule	*request;
};

struct							s_result
{
	t_error			*error;
	t_bresult		*ast;
	t_bresult		*backup_ast;
	int				consumed;
	bool			fatal;
	bool			valid;
};

t_token							*offset_list(t_token *list, int offset);
struct s_result					is_syntax_valid(t_state const prev);

/*
** Rule builders
*/
t_bresult						*simple_command_build(const t_state *state,
												struct s_result *last_build);
t_bresult						*subshell_build(const t_state *state,
												struct s_result *last_build);

t_bresult						*pipe_sequence_build(const t_state *state,
												struct s_result *last_build);
t_bresult						*pipe_sequence_finalize(const t_state *state,
												struct s_result *last_build);

t_bresult						*and_or_build(const t_state *state,
												struct s_result *last_build);
t_bresult						*and_or_finalize(const t_state *state,
												struct s_result *last_build);

t_bresult						*pipe_andor_finalize_right(const t_state *state,
												struct s_result *last_build);
t_bresult						*list_build(const t_state *state,
												struct s_result *last_build);

/*
** Simple command builder auxiliary
*/

bool							is_redirect(t_token *t);
struct s_io_redirect			*get_redirects(t_token *list, int length);

t_node							*ast_new_node(void *value,
												enum e_node_type node_type);
void							ast_free_recursive(t_node *node);

int								tree_get_depth(t_node *parent);
t_bresult						*insert_left_recursive(t_bresult *bresult,
									t_node *parent, t_node *insertion);

extern const t_rule				g_complete_command;
extern const t_rule				g_list;
extern const t_rule				g_list_dash;
extern const t_rule				g_and_or;
extern const t_rule				g_and_or_dash;
extern const t_rule				g_pipeline;
extern const t_rule				g_pipe_sequence;
extern const t_rule				g_pipe_sequence_dash;
extern const t_rule				g_command;
extern const t_rule				g_compound_command;
extern const t_rule				g_subshell;
extern const t_rule				g_compound_list;
extern const t_rule				g_term_rule;
extern const t_rule				g_term_rule_dash;

extern const t_rule				g_if_clause;
extern const t_rule				g_else_part;
extern const t_rule				g_while_clause;

/*
** some rules are omitted because I am a bad programmer. But here they are:
** extern const t_rule	g_name;
** extern const t_rule	g_in;
** extern const t_rule	g_wordlist;
** extern const t_rule	g_wordlist_dash;
** extern const t_rule	g_for_clause;
** extern const t_rule	g_until_clause;
** extern const t_rule	g_function_definition;
** extern const t_rule	g_function_body;
** extern const t_rule	g_fname;
** extern const t_rule	g_case_clause;
** extern const t_rule	g_case_list;
** extern const t_rule	g_case_item_ns;
** extern const t_rule	g_case_item;
** extern const t_rule	g_for_token;
** extern const t_rule	g_if_token;
** extern const t_rule	g_then_token;
** extern const t_rule	g_fi_token;
** extern const t_rule	g_elif_token;
** extern const t_rule	g_else_token;
** extern const t_rule	g_while_token;
** extern const t_rule	g_until_token;
** extern const t_rule	g_do_token;
** extern const t_rule	g_done_token;
** extern const t_rule	g_bang_token;
*/

extern const t_rule				g_brace_group;
extern const t_rule				g_do_group;
extern const t_rule				g_simple_command;
extern const t_rule				g_cmd_name;
extern const t_rule				g_cmd_word;
extern const t_rule				g_cmd_prefix;
extern const t_rule				g_cmd_prefix_dash;
extern const t_rule				g_cmd_suffix;
extern const t_rule				g_cmd_suffix_dash;
extern const t_rule				g_redirect_list;
extern const t_rule				g_redirect_list_dash;
extern const t_rule				g_io_redirect;
extern const t_rule				g_io_file;
extern const t_rule				g_filename;
extern const t_rule				g_io_here;
extern const t_rule				g_here_end;
extern const t_rule				g_newline_list;
extern const t_rule				g_newline_list_dash;
extern const t_rule				g_linebreak;
extern const t_rule				g_separator_op;
extern const t_rule				g_semicolon_list;
extern const t_rule				g_semicolon_list_dash;
extern const t_rule				g_separator;
extern const t_rule				g_sequential_sep;

extern const t_rule				g_semicolon_token;
extern const t_rule				g_ampersand_token;
extern const t_rule				g_newline_token;
extern const t_rule				g_and_if_token;
extern const t_rule				g_or_if_token;
extern const t_rule				g_pipe_token;
extern const t_rule				g_empty_token;
extern const t_rule				g_lbracket_token;
extern const t_rule				g_rbracket_token;
extern const t_rule				g_lbrace_token;
extern const t_rule				g_rbrace_token;
extern const t_rule				g_word_token;
extern const t_rule				g_assignment_word_token;
extern const t_rule				g_io_number_token;
extern const t_rule				g_triless_token;
extern const t_rule				g_dless_token;
extern const t_rule				g_dlessdash_token;

extern const t_rule				g_less_token;
extern const t_rule				g_lessand_token;
extern const t_rule				g_great_token;
extern const t_rule				g_greatand_token;
extern const t_rule				g_dgreat_token;
extern const t_rule				g_lessgreat_token;
extern const t_rule				g_clobber_token;

#endif
