/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_script.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 14:44:44 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/12 16:40:36 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_SCRIPT_H
# define SHELL_SCRIPT_H

# include "libft.h"
# include "ft_printf.h"
# include <stdbool.h>
# include "twenty_one_sh.h"

# define TOKEN_DELIMITERS " \t\r\a"
# define ISQT(x) (((x) == '\'' || (x) == '"' || (x) == '`') ? (1) : (0))

# define LEXER_OUTSIDE_QUOTE       1
# define LEXER_INSIDE_QUOTE        2
# define LEXER_NEXT_ESCAPED        4
# define LEXER_NEXT_UNQUOTED_DELIM 8
# define LEXER_UNQUOTED_PRINT      16
# define LEXER_BREAK               32

/*
** TODO: Add functions capability:
**         global function database
**         lifespan control
*/

/*
** Do not change order in which these entries appear. They provide easy random
** access to a specific token entry in recognisable by this shell token table.
** @see g_tokens[] src/lexer/tokenizer.c
*/

enum						e_token_type
{
	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,

	TOKEN_SEMICOLON,
	TOKEN_AND_IF,
	TOKEN_OR_IF,

	TOKEN_TRILESS,
	TOKEN_DLESSDASH,
	TOKEN_DLESS,
	TOKEN_DGREAT,
	TOKEN_LESSAND,
	TOKEN_GREATAND,
	TOKEN_LESSGREAT,
	TOKEN_CLOBBER,
	TOKEN_PIPE,

	TOKEN_LESS,
	TOKEN_GREAT,
	TOKEN_AMPERSAND,
	TOKEN_BANG,

	TOKEN_IF,
	TOKEN_THEN,
	TOKEN_ELSE,
	TOKEN_ELIF,
	TOKEN_FI,
	TOKEN_DO,
	TOKEN_DONE,
	TOKEN_WHILE,
	TOKEN_UNTIL,

	TOKEN_WORD,
	TOKEN_NAME,
	TOKEN_IO_NUMBER,
	TOKEN_ASSIGNMENT_WORD,

	TOKEN_EXPANSION,
	TOKEN_BEXPANSION,

	TOKEN_NEWLINE,
	TOKEN_EMPTY,

	TOKEN_NOT_APPLICABLE,

	TOKEN_KEYWORD,
	TOKEN_WORD_COMMAND
};

enum						e_node_type
{
	NODE_PIPE,
	NODE_SUBSHELL,
	NODE_SEPARATOR,
	NODE_AMPERSAND,
	NODE_OR_IF,
	NODE_AND_IF,
	NODE_LOOP_WHILE,
	NODE_LOOP_FOR,
	NODE_LOOP_UNTIL,
	NODE_COMMAND,
};

/*
** s_lexer_token is a struct that used on lexical analysis stage. Lexer uses
** information from g_tokens table to parse char input into tokens and literals
** and pass this new stream to syntax analyzer and then parser.
*/

struct						s_lexer_token
{
	char					*text;
	char					*token_name;
	enum e_token_type		type;
	bool					requires_single;
};

typedef struct				s_token
{
	int						line_nbr;
	const char				*value;
	enum e_token_type		type;
	struct s_token			*prev;
	struct s_token			*next;
}							t_token;

typedef struct				s_node
{
	struct s_command		*command;
	enum e_node_type		node_type;
	bool					is_async;
	struct s_node			*left;
	struct s_node			*right;
}							t_node;

struct						s_io_rdr_param
{
	char					*path;
	int						fd;
};

typedef struct				s_io_redirect
{
	struct s_io_rdr_param	left;
	struct s_io_rdr_param	right;
	enum e_token_type		type;
}							t_io_rdr;

typedef struct				s_command
{
	char		**args;
	char		**assignments;
	t_io_rdr	*io_redirects;
}							t_command;

union						u_executor
{
	int		(*exec)(const t_node *node);
	int		(*exec_alt_context)(const t_node *node, struct s_context *context);
};

struct						s_executor
{
	enum e_node_type	node_type;
	union u_executor	executor;
};

extern const struct s_lexer_token	g_tokens[];
extern const struct s_executor		g_executors_table[];

/*
** Lexer
*/

struct s_token				*tokenize(char *str, const char *delimiters);
struct s_token				*new_token(char *value, enum e_token_type type);
void						add_token(t_token **head, t_token **tail,
														t_token *to_add);
t_token						*pop_token(t_token **head, t_token **tail);
void						free_token(struct s_token *token);

enum e_token_type			token_class_contextual(const char *str,
														enum e_token_type prev);
void						free_array(void **array);

/*
** Lexer Preprocess
*/

char						*strip_escaped_nl_and_comments(char *string);

/*
** AST Preprocess
*/

void						run_heredocs(t_node *node);

/*
** AST Main
*/

void						run_script(t_token *list_head, bool log_recursion);
int							exec_command(const t_node *command_node,
	struct s_context *new_context);
int							exec_subshell(const t_node *node,
	struct s_context *new_context);
int							exec_semicolon_iterative(t_node *parent);
int							exec_semicolon_recursive(const t_node *parent);
int							exec_and_if(const t_node *parent);
int							exec_or_if(const t_node *parent);
int							exec_node(const t_node *node);
int							exec_abort(int dummy);
int							exec_pipeline(const t_node *node);

/*
** File reading and executing
*/

int							read_filename(const char *file, char **data);

void						rdr_heredocs(t_context *context, t_io_rdr *rdrs);
int							alterate_filedes(const struct s_command *command,
	t_context *cntxt);

/*
** Expansions
*/

char						*expand(char *string);

#endif
