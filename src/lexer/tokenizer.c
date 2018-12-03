#include "shell_script.h"
#include "shell_builtins.h"

char			*g_keywords[] = {
	"while",
	"do",
	"done",
	"if",
	"then",
	"fi",
	"elif",
	"else",
	"for",
	"in",
	"until",
	"case",
	"esac",
	NULL
};

char			*g_operators[] = {
	"&&", "||", ">>", "<<",
	"|", "<", ">", "+", "-", "/", "*",
	NULL
};

struct s_token	*get_token(char *string)
{
	extern struct s_builtin	g_builtins[];
	struct s_token			*token;
	unsigned long			i;

	token = NULL;
	i = 0;
	while (!token && g_keywords[i] != NULL)
		if (ft_strcmp(g_keywords[i++], string) == 0)
			token = new_token(string, TOKEN_KEYWORD);
	i = 0;
	while (!token && g_builtins[i].name != NULL)
		if (ft_strcmp(g_builtins[i++].name, string) == 0)
			token = new_token(string, COMMAND);
	i = 3;
	while (!token && g_singles[i] != NULL)
		if (ft_strcmp(g_singles[i++], string) == 0)
			token = new_token(string, BRACE);
	i = 0;
	while (!token && g_operators[i] != NULL)
		if (ft_strcmp(g_operators[i++], string) == 0)
			token = new_token(string, OPERATOR);
	if (token == NULL && ft_strcmp(string, ";") == 0)
		token = new_token(string, SEPARATOR);
	if (token == NULL)
		token = new_token(string, LITERAL);
	return (token);
}

struct s_token	*tokenize(char **array)
{
	struct s_token	*head;
	struct s_token	*tail;
	unsigned long	i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (array && array[i])
		add_token(&head, &tail, get_token(array[i++]));
	add_token(&head, &tail, new_token(";", SEPARATOR));
	return (head);
}

char	*enum_to_str(enum e_token e)
{
	switch (e)
	{
		case TOKEN_KEYWORD:
			return ("KEYWORD");
		case LITERAL:
			return ("LITERAL");
		case BRACE:
			return ("BRACE");
		case REDIRECTION:
			return ("REDIRECTION");
		case TOKEN_PIPE:
			return ("PIPE");
		case OPERATOR:
			return ("OPERATOR");
		case COMMAND:
			return ("COMMAND");
		case SEPARATOR:
			return ("SEPARATOR");
		default:
			return ("null");
	}
}

int main_(int argc, char **argv)
{
	t_token	*tokens;
	//char	*name;
	char	*line;

	*argv = argv[argc - argc];
	while (get_next_line(STDIN_FILENO, &line) != 1)
		;
	tokens = tokenize(smart_split(line, LIBFT_WHTSP));
	while (tokens)
	{
		ft_printf("(%s, %s) ", enum_to_str(tokens->type), tokens->value);
		tokens = tokens->next;
	}
	return (0);
}