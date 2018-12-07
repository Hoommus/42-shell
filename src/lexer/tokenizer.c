#include "shell_script.h"
#include "shell_builtins.h"

char	*next_token(const char *str, const char *delims)
{
	u_int64_t	i;
	char		c;
	char		*token;

	i = 0;
	c = str[i];
	if (ISQT(c) && ++i)
		while (str[i] && str[i++] != c)
			;
	else
		while (str[i] && ft_strchr(delims, str[i]) == NULL)
			i++;
	if (i == 0)
		ft_printf("error with {%s}\n", str);
	token = ft_strsub(str, 0, i);
	return (token);
}

struct s_token	*create_token(const char *str)
{
	enum e_token_type	type;
	int					i;

	i = TOKEN_IF; // should be 0
	type = TOKEN;
	while (g_tokens[i].token_name != NULL)
	{
		if (ft_strcmp(g_tokens[i].text, str) == 0)
		{
			type = g_tokens[i].type;
			return (new_token(ft_strnew(0), type));
		}
		i++;
	}
	return (new_token((char *)str, type));
}

struct s_token	*tokenizer(const char *string, const char *delimiters)
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
		i += ft_strlen(token_value);
		free(token_value);
	}
	return (head);
}

int main_(int argc, char **argv)
{
	t_token	*tokens;
	t_token	*head;
	char	*line;

	*argv = argv[argc - argc];
	while (get_next_line(STDIN_FILENO, &line) != 1)
		;
	head = tokenizer(line, TOKEN_DELIMITERS);
	tokens = head;
	while (tokens)
	{
		if (tokens->type != TOKEN)
			ft_printf("(%s) ", g_tokens[tokens->type].token_name);
		else
			ft_printf("(%s %s) ", g_tokens[tokens->type].token_name, tokens->value);
		tokens = tokens->next;
	}
	system("leaks 21sh");
	return (0);
}