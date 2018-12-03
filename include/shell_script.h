/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_script.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 14:44:44 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/03 19:31:53 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_SCRIPT_LANG_H
# define SHELL_SCRIPT_LANG_H

# include "libft.h"
# include <stdbool.h>

# define TOKEN_DELIMITERS "\n\t;'\" \r\a"
# define ISQT(x) (((x) == '\'' || (x) == '"' || (x) == '`') ? (1) : (0))

extern char					*g_singles[];
extern char					*g_keywords[];
extern char					*g_operators[];

/*
** Do not change order in which these entries appear. They provide access to a
** specific token entry in g_tokens[] table.
*/

enum						e_token
{
	TOKEN_IF,
	TOKEN_THEN,
	TOKEN_ELSE,
	TOKEN_ELIF,
	TOKEN_FI,
	TOKEN_DO,
	TOKEN_DONE,
	TOKEN_CASE,
	TOKEN_ESAC,
	TOKEN_WHILE,
	TOKEN_UNTIL,
	TOKEN_FOR,

	TOKEN_IN,

	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,
	TOKEN_LSQBRACKET,
	TOKEN_RSQBRACKET,

	TOKEN_SEMICOLON,
	TOKEN_PIPE,
	TOKEN_BANG,
	TOKEN_AND_IF,
	TOKEN_OR_IF,
	TOKEN_DSEMI,
	TOKEN_DLESS,
	TOKEN_DGREAT,
	TOKEN_LESSAND,
	TOKEN_GREATAND,
	TOKEN_LESSGREAT,
	TOKEN_DLESSDASH,
	TOKEN_CLOBBER,

	TOKEN_LESS,
	TOKEN_GREAT,
	TOKEN_NEWLINE,
	TOKEN_AMPERSAND,
/*
** Add token classes below and after "newline" token in g_tokens[] table
*/

	TOKEN_WORD,
	TOKEN_NAME,
	TOKEN_IO_NUMBER,
	TOKEN_ASSIGNMENT_WORD,
	TOKEN_EMPTY,

	TOKEN_KEYWORD,

	LITERAL,
	LITERAL_ARRAY,
	BRACE,
	REDIRECTION,
	OPERATOR,


	SEPARATOR,
	COMMAND,
	VARIABLE,
	TOKEN_TILDE,
	END_OF_SCRIPT,
};

/*
** s_parse_token is a struct that used on parsing time. Lexer uses information
** from g_tokens table to parse char input into tokens and literals and pass
** this new stream to syntax analyzer and then parser.
*/

struct						s_parse_token
{
	char			*text;
	char			*token_name;
	enum e_token	type;
	bool			requires_single;
};

extern struct s_parse_token	g_tokens[];

/*
** Syntax rule is an entity that contains an array of possible expansions
** which define language grammar.
**
** Rules which given rule expands to are stored in 'expands_to' field.
** The expands_to field is a two dimensional NULL-terminated array of pointers
** to s_syntax_rule variables declared in syntax_rules.h.
** Basically, it can hold up to 9 cases of given rule each of which
*А* can hold up to 9 rules or tokens to which this rule expands.
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
**          { &simple_command, 0 },      // command can be solely simple_command
**          { &compound_command, 0 },    // OR compound_command
**          // OR compound_command which is followed by redirect_list
**          { &compound_command, &redirect_list, 0 },
**          { &function_definition, 0 }, // OR be a function definition
**          { NULL } // just an array terminator
**      }
**  };
*/

struct						s_syntax_rule
{
	enum e_token				representation[2];
	const struct s_syntax_rule	*expands_to[10][10];
};

typedef struct				s_token
{
	const char		*value;
	enum e_token	type;
	struct s_token	*next;
}							t_token;

typedef enum				e_type
{
	STRING,
	FLOAT,
	INTEGER,
	NOT_APPLICABLE
}							t_datatype;

typedef struct				s_node
{
	void			*value;
	enum e_token	token_type;
	enum e_type		data_type;
	struct s_node	*left;
	struct s_node	*right;
}							t_node;

/*
** Lexer
*/
struct s_token				*tokenize(char **array);
struct s_token				*new_token(char *value, enum e_token type);
void						add_token(t_token **head, t_token **tail,
														t_token *to_add);
void						free_token(struct s_token *token);

char						**smart_split(char *str, char *delimiters);

/*
** File reading and executing
*/

#endif
