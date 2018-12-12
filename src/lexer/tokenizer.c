#include "shell_script.h"
#include "shell_script_parser.h"

struct s_parser_state	g_parser_state;

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

static char				*next_token(const char *str, const char *delims)
{
	u_int64_t	i;
	char		c;

	i = 0;
	c = str[i];
	if (ISQT(c) && ++i)
		while (str[i] && ft_strchr(delims, str[i++]) == NULL)
			;
	else if ((i = is_separate(str)))
		return (ft_strsub(str, 0, i));
	else
		while (str[i] && ft_strchr(delims, str[i]) == NULL
						&& !is_separate(str + i))
			i++;
	return (ft_strsub(str, 0, i));
}

static struct s_token	*create_token(const char *str)
{
	enum e_token_type	type;

	type = get_token_type_contextual(str);
	return (new_token(ft_strdup(str), type));
}

struct s_token		*tokenize(const char *string, const char *delimiters)
{
	struct s_token	*head;
	struct s_token	*tail;
	char			*token_value;
	u_int64_t		i;

	head = NULL;
	tail = NULL;
	i = -1;
	while (string && string[++i])
	{
		if (ft_strchr(delimiters, string[i]) != NULL)
			continue ;
		token_value = next_token(string + i, delimiters);
		add_token(&head, &tail, create_token(token_value));
		i += ft_strlen(token_value) - 1;
		free(token_value);
		g_parser_state.list_head = head;
		g_parser_state.list_tail = tail;
	}
	if (!tail || (tail->type != TOKEN_SEMICOLON && tail->type != TOKEN_NEWLINE))
		add_token(&head, &tail, create_token(";"));
	return (head);
}
