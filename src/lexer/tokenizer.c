/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:55:49 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/28 16:07:54 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

/*
** case syntax structure is avoided.
** But here its tokens are, just in case (no pun intended)
** {"case",          "case",        TOKEN_CASE,            false},
** {"esac",          "esac",        TOKEN_ESAC,            false},
** {";;",            "DSEMI",       TOKEN_DSEMI,           true },
*/

const struct s_lexer_token	g_tokens[] = {
	{"if",            "IF",          TOKEN_IF,              false},
	{"then",          "THEN",        TOKEN_THEN,            false},
	{"else",          "ELSE",        TOKEN_ELSE,            false},
	{"elif",          "ELIF",        TOKEN_ELIF,            false},
	{"fi",            "FI",          TOKEN_FI,              false},
	{"do",            "DO",          TOKEN_DO,              false},
	{"done",          "DONE",        TOKEN_DONE,            false},
	{"while",         "WHILE",       TOKEN_WHILE,           false},
//	{"until",         "UNTIL",       TOKEN_UNTIL,           false},
//	{"for",           "FOR",         TOKEN_FOR,             false},

//	{"in",            "IN",          TOKEN_IN,              false},

	{"{",             "LBRACE",      TOKEN_LBRACE,          true },
	{"}",             "RBRACE",      TOKEN_RBRACE,          true },
	{"(",             "LBRACKET",    TOKEN_LBRACKET,        true },
	{")",             "RBRACKET",    TOKEN_RBRACKET,        true },
//	{"[",             "LSQBRACKET",  TOKEN_LSQBRACKET,      true },
//	{"]",             "RSQBRACKET",  TOKEN_RSQBRACKET,      true },

	{";",             "SEMICOLON",   TOKEN_SEMICOLON,       true },
	{"!",             "BANG",        TOKEN_BANG,            true },
	{"&&",            "AND_IF",      TOKEN_AND_IF,          true },
	{"||",            "OR_IF",       TOKEN_OR_IF,           true },

	{"<<-",           "DLESSDASH",   TOKEN_DLESSDASH,       true },
	{"<<",            "DLESS",       TOKEN_DLESS,           true },
	{">>",            "DGREAT",      TOKEN_DGREAT,          true },
	{"<&",            "LESSAND",     TOKEN_LESSAND,         true },
	{">&",            "GREATAND",    TOKEN_GREATAND,        true },
	{"<>",            "LESSGREAT",   TOKEN_LESSGREAT,       true },
	{">|",            "CLOBBER",     TOKEN_CLOBBER,         true },
	{"|",             "PIPE",        TOKEN_PIPE,            true },

	{"<",             "LESS",        TOKEN_LESS,            true },
	{">",             "GREAT",       TOKEN_GREAT,           true },
	{"~",             "TILDE",       TOKEN_TILDE,           false},
	{"&",             "AMPERSAND",   TOKEN_AMPERSAND,       true },

	{"^~/?[\\/\\w]*", "WORD",        TOKEN_WORD,            false},
	{"^\\D\\w+",      "NAME",        TOKEN_NAME,            false},
	{"\\d*[><]\\d*",  "IO_NUMBER",   TOKEN_IO_NUMBER,       false},
	{"^\\w+\\=",      "ASSIGNMENT",  TOKEN_ASSIGNMENT_WORD, false},

	{"\n",            "NEWLINE",     TOKEN_NEWLINE,         true },
	{"!",             "EMPTY_LOL",   TOKEN_EMPTY,           false},
	{"!",             "literal",     TOKEN_NOT_APPLICABLE,  false},
	{"!",             "COMMAND",     TOKEN_WORD_COMMAND,    false},

	{NULL,            NULL,          TOKEN_KEYWORD,         false},
};

static size_t			is_separate(const char *str)
{
	int		i;

	i = TOKEN_LBRACE;
	while (g_tokens[++i].token_name != NULL)
		if (g_tokens[i].requires_single &&
			ft_strncmp(str, g_tokens[i].text, ft_strlen(g_tokens[i].text)) == 0)
			return (ft_strlen(g_tokens[i].text));
	return (0);
}

/*
** Awfully hardcoded solution for exceptions
*/

u_int64_t		is_exception(const char *str)
{
	u_int64_t	i;

	if (!str)
		return (0);
	i = 0;
	if ((str[0] == '>' || str[0] == '<') &&
		(ft_isalpha(str[1]) || ft_strchr("/\\\"\'`", str[1])))
		return (1);
	while (ft_isdigit(str[i]))
		i++;
	if (i > 0 && ft_strchr("<>", str[i]) && !is_separate(str + i))
		return (i + 1);
	return (0);
}

static u_int64_t		get_quoted_size(const char *str, const char *delims)
{
	u_int64_t	i;
	int			state;
	char		quote;

	i = 0;
	quote = str[i];
	state = LEXER_INSIDE_QUOTE;
	while (str[++i] && state != LEXER_OUTSIDE_QUOTE
					&& state != LEXER_NEXT_UNQUOTED_DELIM)
	{
		if (ft_strchr(delims, str[i + 1]) == NULL
			&& str[i + 1] != '\n' && state != LEXER_INSIDE_QUOTE)
			break ;
		else if (state == LEXER_NEXT_ESCAPED)
			state = LEXER_INSIDE_QUOTE;
		else if (str[i] == '\\')
			state = LEXER_NEXT_ESCAPED;
		else if (str[i] == quote)
		{
			if (ft_strchr(delims, str[i + 1]) == NULL && str[i + 1] != '\n')
				state = LEXER_OUTSIDE_QUOTE;
			else
				state = LEXER_NEXT_UNQUOTED_DELIM;
		}
	}
	return (i);
}

static char				*next_token(const char *str, const char *delims)
{
	u_int64_t	i;
	char		c;

	i = 0;
	c = str[i];
	if (ISQT(c) && ++i)
		i = get_quoted_size(str, delims);
	else if ((i = is_separate(str)))
		return (ft_strsub(str, 0, i));
	else
		while (str[i] && ft_strchr(delims, str[i]) == NULL
			&& !is_separate(str + i))
			i += str[i] == '\\' ? 2 : 1;
	return (ft_strsub(str, 0, i));
}


/*
** Defaults to TOKEN_SEMICOLON, because it does not affect any contextual
** classification
*/
static struct s_token	*create_token(const char *str, t_token *last, int line)
{
	enum e_token_type	type;
	t_token				*token;

	type = token_class_contextual(str, last == NULL ? TOKEN_SEMICOLON
													: last->type);
	token = new_token(ft_strdup(str), type);
	token->line_nbr = line;
	return (token);
}

/*
** Only moves characters around, no memory reallocation bullshit.
*/

static char				*strip_escaped_nl(char *string)
{
	size_t	length;
	size_t	i;

	length = ft_strlen(string);
	i = 0;
	while (length && i < length - 1)
	{
		if (string[i] == '\\' && string[i + 1] == '\n')
			ft_memmove(string + i, string + i + 2, length - i - 2);
		else
			i++;
	}
	return (string);
}

char				*strip_comments(char *string)
{
	u_int64_t	i;

	i = 0;
	while (string[i])
	{
		if (string[i] == '#')
			while (string[i] && string[i] != '\n')
				string[i++] = ' ';
		i++;
	}
	return (string);
}

char				*strip_non_printable(char *string)
{
	// TODO
	return (string);
}

/*
** Splits string into token list and updates corresponding global state.
*/

struct s_token		*tokenize(char *string, const char *delimiters)
{
	struct s_token	*head;
	struct s_token	*tail;
	char			*token_value;
	u_int64_t		i;
	int				line;

	head = NULL;
	tail = NULL;
	line = 1;
	i = -1;
	strip_escaped_nl(strip_comments(string));
	while (string && string[++i])
	{
		if (ft_strchr(delimiters, string[i]) != NULL)
			continue ;
		token_value = next_token(string + i, delimiters);
		line += ft_strcmp(token_value, "\n") == 0 ? 1 : 0;
		add_token(&head, &tail,
			create_token(token_value, pop_token(&head, &tail), line));
		i += ft_strlen(token_value) - 1;
		free(token_value);
	}
	if (!tail || (tail->type != TOKEN_SEMICOLON && tail->type != TOKEN_NEWLINE))
		add_token(&head, &tail, create_token("\n", tail, line));
	return (head);
}
