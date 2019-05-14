/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:55:49 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/13 21:54:20 by vtarasiu         ###   ########.fr       */
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
** {"if", "IF", TOKEN_IF, false},
** {"then", "THEN", TOKEN_THEN, false},
** {"else", "ELSE", TOKEN_ELSE, false},
** {"elif", "ELIF", TOKEN_ELIF, false},
** {"fi", "FI", TOKEN_FI, false},
** {"do", "DO", TOKEN_DO, false},
** {"done", "DONE", TOKEN_DONE, false},
** {"while", "WHILE", TOKEN_WHILE, false},
** {"until", "UNTIL", TOKEN_UNTIL, false},
** {"for", "FOR", TOKEN_FOR, false},
** {"in", "IN", TOKEN_IN, false},
** {"[", "LSQBRACKET", TOKEN_LSQBRACKET, true },
** {"]", "RSQBRACKET", TOKEN_RSQBRACKET, true },
** {"!", "BANG", TOKEN_BANG, true },
*/

const struct s_lexer_token	g_tokens[] = {
	{"{", "LBRACE", TOKEN_LBRACE, false},
	{"}", "RBRACE", TOKEN_RBRACE, false},
	{"(", "LBRACKET", TOKEN_LBRACKET, false},
	{")", "RBRACKET", TOKEN_RBRACKET, false},

	{";", "SEMICOLON", TOKEN_SEMICOLON, true },
	{"&&", "AND_IF", TOKEN_AND_IF, true },
	{"||", "OR_IF", TOKEN_OR_IF, true },

	{"<<<", "TRILESS", TOKEN_TRILESS, true },
	{"<<-", "DLESSDASH", TOKEN_DLESSDASH, true },
	{"<<", "DLESS", TOKEN_DLESS, true },
	{">>", "DGREAT", TOKEN_DGREAT, true },
	{"<&", "LESSAND", TOKEN_LESSAND, true },
	{">&", "GREATAND", TOKEN_GREATAND, true },
	{"<>", "LESSGREAT", TOKEN_LESSGREAT, true },
	{">|", "CLOBBER", TOKEN_CLOBBER, true },
	{"|", "PIPE", TOKEN_PIPE, true },

	{"<", "LESS", TOKEN_LESS, true },
	{">", "GREAT", TOKEN_GREAT, true },
	{"&", "AMPERSAND", TOKEN_AMPERSAND, true },

	{"^~/?[\\/\\w]*", "WORD", TOKEN_WORD, false},
	{"^\\D\\w+", "NAME", TOKEN_NAME, false},
	{"\\d*[><]\\d*", "IO_NUMBER", TOKEN_IO_NUMBER, false},
	{"^\\w+\\=", "ASSIGNMENT", TOKEN_ASSIGNMENT_WORD, false},

	{"$(", "EXPANSION", TOKEN_EXPANSION, false},
	{"${", "EXPANSION", TOKEN_BEXPANSION, false},
	{"\n", "NEWLINE", TOKEN_NEWLINE, true },
	{"!", "EMPTY_LOL", TOKEN_EMPTY, false},
	{"!", "literal", TOKEN_NOT_APPLICABLE, false},
	{"!", "COMMAND", TOKEN_WORD_COMMAND, false},

	{NULL, NULL, TOKEN_KEYWORD, false},
};

static size_t				is_separate(const char *str)
{
	int		i;

	i = TOKEN_LBRACE;
	while (g_tokens[++i].token_name != NULL)
		if (g_tokens[i].requires_single &&
			ft_strncmp(str, g_tokens[i].text, ft_strlen(g_tokens[i].text)) == 0)
			return (ft_strlen(g_tokens[i].text));
	return (0);
}

static u_int64_t			get_quoted_size(const char *str, const char *delims)
{
	u_int64_t	i;
	int			state;
	char		quote;

	i = 0;
	quote = str[i];
	state = LEXER_INSIDE_QUOTE;
	while (str[++i] && state != LEXER_BREAK)
	{
		if (str[i] == '\\')
			i++;
		else if (state == LEXER_UNQUOTED_PRINT &&
			(ft_strchr(delims, str[i + 1]) != NULL || str[i + 1] == '\n'))
			state = LEXER_BREAK;
		else if (str[i] == quote)
		{
			if (ft_strchr(delims, str[i + 1]) == NULL && str[i + 1] != '\n'
				&& !ISQT(str[i + 1]))
				state = LEXER_UNQUOTED_PRINT;
			else
				state = LEXER_BREAK;
		}
	}
	return (i);
}

static char					*next_token(const char *str, const char *delims)
{
	u_int64_t	i;
	size_t		len;
	char		c;

	i = 0;
	c = str[i];
	len = ft_strlen(str);
	if (ISQT(c) && ++i)
		i = get_quoted_size(str, delims);
	else if ((i = is_separate(str)))
		return (ft_strsub(str, 0, i));
	else
		while (i < len && ft_strchr(delims, str[i]) == NULL
			&& !is_separate(str + i))
			if (ISQT(str[i]))
			{
				i += get_quoted_size(str + i, delims);
				break ;
			}
			else
				i += str[i] == '\\' ? 2 : 1;
	return (ft_strsub(str, 0, i));
}

/*
** Defaults to TOKEN_SEMICOLON, because it does not affect any contextual
** classification
*/

static struct s_token		*create_token(const char *str, t_token *last,
	int line)
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
** Splits string into token list and updates corresponding global state.
*/

struct s_token				*tokenize(char *string, const char *delimiters)
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
	strip_escaped_nl_and_comments(string);
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
