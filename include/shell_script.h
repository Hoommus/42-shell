#ifndef SCRIPT_LANG_H
# define SCRIPT_LANG_H

# include "twenty_one_sh.h"

# define TOKEN_DELIMITERS "\n\t;'\" \r\a"
# define ISQT(x) (((x) == '\'' || (x) == '"' || (x) == '`') ? (1) : (0))

extern char		*g_singles[];
extern char		*g_keywords[];
extern char		*g_operators[];

extern struct s_parse_token	*g_map[];

enum			e_token
{
	KEYWORD,		// while, if, fi, do, done, etc;
	LITERAL,		// "hello", 42, 10.5;
	LITERAL_ARRAY,	// ["hello", "42", "10.5"]
	BRACE,			// (, ), [, ], {, }
	REDIRECTION,	// 2>&-
	PIPE,			// '|'
	OPERATOR,		// DON'T TEASE ME,
					// OPERATOR, WHAT'S GOING ON?
					// Got it?
	SEPARATOR,		// ; '\n'
	COMMAND,		// string representing a command name;
	END_OF_SCRIPT	// DO AS HE SAYS
};

struct			s_parse_token
{
	char	*text;
	char	*token_name;
	bool	requires_single;
};

typedef struct	s_token
{
	char			*value;
	enum e_token	type;
	struct s_token	*next;
}				t_token;

typedef enum	e_type
{
	STRING,
	FLOAT,
	INTEGER
}				t_datatype;

typedef struct	s_node
{
	void			*value;
	enum e_token	token_type;
	enum e_type		data_type;
	struct s_node	*left;
	struct s_node	*right;
}				t_node;

/*
** Lexer
*/
struct s_token	*tokenize(char **array);
struct s_token	*new_token(char *value, enum e_token type);
void			add_token(t_token **head, t_token **tail, t_token *to_add);
void			free_token(struct s_token *token);

char			**smart_split(char *str, char *delimiters);

/*
** File reading and executing
*/

/*
** Hullo?
*/

#endif
