#include "shell_script.h"
#include "shell_script_parser.h"

struct s_parser_state	g_ps;

static size_t			is_separate(const char *str)
{
	int		i;

	i = TOKEN_IN;
	while (g_tokens[++i].token_name != NULL)
		if (g_tokens[i].requires_single &&
			ft_strncmp(str, g_tokens[i].text, ft_strlen(g_tokens[i].text)) == 0)
			return (ft_strlen(g_tokens[i].text));
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

static struct s_token	*create_token(const char *str)
{
	enum e_token_type	type;

	type = get_token_type_contextual(str);
	return (new_token(ft_strdup(str), type));
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

	head = NULL;
	tail = NULL;
	i = -1;
	strip_escaped_nl(strip_comments(string));
	while (string && string[++i])
	{
		if (ft_strchr(delimiters, string[i]) != NULL)
			continue ;
		token_value = next_token(string + i, delimiters);
		add_token(&head, &tail, create_token(token_value));
		i += ft_strlen(token_value) - 1;
		free(token_value);
	}
	if (!tail || (tail->type != TOKEN_SEMICOLON && tail->type != TOKEN_NEWLINE))
		add_token(&head, &tail, create_token("\n"));
	g_ps.list_head = head;
	g_ps.list_tail = tail;
	return (head);
}
