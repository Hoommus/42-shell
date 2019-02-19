/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_script.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 14:44:44 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/17 14:51:33 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_SCRIPT_H
# define SHELL_SCRIPT_H

# include "libft.h"
# include "ft_printf.h"
# include <stdbool.h>

# define TOKEN_DELIMITERS " \t\r\a"
# define ISQT(x) (((x) == '\'' || (x) == '"' || (x) == '`') ? (1) : (0))

# define LEXER_OUTSIDE_QUOTE       0
# define LEXER_INSIDE_QUOTE        1
# define LEXER_NEXT_ESCAPED        2
# define LEXER_NEXT_UNQUOTED_DELIM 4
# define LEXER_NEXT_UNQUOTED_PRINT 8

/*
** TODO: Add functions capability:
**         global function database
**         lifespan control
*/

/*
** Do not change order in which these entries appear. They provide easy random
** access to a specific token entry in g_tokens[] table.
*/

enum						e_token_type
{
	TOKEN_IF,
	TOKEN_THEN,
	TOKEN_ELSE,
	TOKEN_ELIF,
	TOKEN_FI,
	TOKEN_DO,
	TOKEN_DONE,
	TOKEN_WHILE,
//	TOKEN_UNTIL,
//	TOKEN_FOR,

//	TOKEN_IN,

	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,
//	TOKEN_LSQBRACKET,
//	TOKEN_RSQBRACKET,

	TOKEN_SEMICOLON,
	TOKEN_BANG,
	TOKEN_AND_IF,
	TOKEN_OR_IF,

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
	TOKEN_TILDE,
	TOKEN_AMPERSAND,

	TOKEN_WORD,
	TOKEN_NAME,
	TOKEN_IO_NUMBER,
	TOKEN_ASSIGNMENT_WORD,

	TOKEN_NEWLINE,
	TOKEN_EMPTY,

	TOKEN_NOT_APPLICABLE,

	TOKEN_KEYWORD,
	TOKEN_WORD_COMMAND,

	LITERAL,
	LITERAL_ARRAY,
	BRACE,
	REDIRECTION,
	OPERATOR,

	SEPARATOR,
	AST_COMMAND,
	VARIABLE,
	END_OF_SCRIPT,
};

/*
** s_parse_token is a struct that used on parsing time. Lexer uses information
** from g_tokens table to parse char input into tokens and literals and pass
** this new stream to syntax analyzer and then parser.
*/

struct						s_parse_token
{
	char				*text;
	char				*token_name;
	enum e_token_type	type;
	bool				requires_single;
};

extern const struct s_parse_token	g_tokens[];

/*
** TODO: Add information about a line where specific token resides
*/
typedef struct				s_token
{
	int					line_nbr;
	const char			*value;
	enum e_token_type	type;
	struct s_token		*prev;
	struct s_token		*next;
}							t_token;

enum						e_node_type
{
	NODE_GENERIC,
	NODE_STRING,
	NODE_COMMAND,
};

typedef enum				e_type
{
	STRING,
	FLOAT,
	INTEGER,
	NOT_APPLICABLE
}							t_datatype;

typedef struct				s_node
{
	void				*value;
	enum e_token_type	token_type;
	enum e_node_type	node_type;
	struct s_node		*left;
	struct s_node		*right;
}							t_node;

struct						s_io_redirect
{
	short	fd_what;
	short	fd_where;
	bool	append;
};

struct						s_command
{
	char					**args;
	t_token					*assignments;
	struct s_io_redirect	*io_redirects;
	bool					is_bg;
};

/*
** Lexer
*/
struct s_token				*tokenize(char *str, const char *delimiters);
struct s_token				*new_token(char *value, enum e_token_type type);
void						add_token(t_token **head, t_token **tail,
														t_token *to_add);
void						free_token(struct s_token *token);

enum e_token_type			token_class_contextual(const char *str,
														enum e_token_type prev);
char						**smart_split(char *str, char *delimiters);

void						free_array(char **array);

void						run_script(t_token *list_head, bool log_recursion);
/*
** File reading and executing
*/
int							read_filename(char *file, char **data);

#endif
