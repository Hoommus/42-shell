#ifndef TWENTY_ONE_SH_SCRIPT_LANG_H
# define TWENTY_ONE_SH_SCRIPT_LANG_H

# define TOKEN_DELIMITERS "\n\t;'\" \r\a"

# define ISQT(x) (((x) == '\'' || (x) == '"' || (x) == '`') ? (1) : (0))

extern char		*g_keywords[];
extern char		*g_operators[];

enum			e_token
{
	KEYWORD,			// while, if, fi, do, done, etc;
	LITERAL,			// "hello", 42, 10.5;
	BRACE,				// (, ), [, ], {, }
	UNARY_OPERATOR,		// -, $
	BINARY_OPERATOR,	// <, >, +, -, /, *, &&, ||
	SEPARATOR,			// ;
	EXPRESSION,			// entity, that has a COMMAND or OPERATOR on it's left
						// and LITERALS as arguments on the right;
	COMMAND,			// string representing a command name;
	DUMMY				// dummy for before parsing
};

struct			s_token
{
	char			*value;
	enum e_token	type;
	struct s_token	*next;
};

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
	struct s_node	*child;
	struct s_node	*siblings;
}				t_node;

char			**smart_split(char *str, char *delimiters);

/*
** Lexer
*/
struct s_token	*new_token(char *value);
void			free_token(struct s_token *token);

#endif
