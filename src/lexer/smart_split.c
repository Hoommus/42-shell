/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 14:44:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/03 18:40:26 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "syntax_rules.h"

#include <assert.h>
char		*g_singles[] = {
	"|",
	";",
	"\n",
	"(",
	")",
	"[",
	"]",
	"{",
	"}",
	NULL
};

struct s_parse_token	g_tokens[] = {
	{"if",            "if",          TOKEN_IF,              false},
	{"then",          "then",        TOKEN_THEN,            false},
	{"else",          "else",        TOKEN_ELSE,            false},
	{"elif",          "elif",        TOKEN_ELIF,            false},
	{"fi",            "fi",          TOKEN_FI,              false},
	{"do",            "do",          TOKEN_DO,              false},
	{"done",          "done",        TOKEN_DONE,            false},
	{"case",          "case",        TOKEN_CASE,            false},
	{"esac",          "esac",        TOKEN_ESAC,            false},
	{"while",         "while",       TOKEN_WHILE,           false},
	{"until",         "until",       TOKEN_UNTIL,           false},
	{"for",           "for",         TOKEN_FOR,             false},

	{"in",            "in",          TOKEN_IN,              false},

	{"{",             "lbrace",      TOKEN_LBRACE,          true },
	{"}",             "rbrace",      TOKEN_RBRACE,          true },
	{"(",             "lbracket",    TOKEN_LBRACKET,        true },
	{")",             "rbracket",    TOKEN_RBRACKET,        true },
	{"[",             "lsqbracket",  TOKEN_LSQBRACKET,      true },
	{"]",             "rsqbracket",  TOKEN_RSQBRACKET,      true },
	{";",             "semicolon",   TOKEN_SEMICOLON,       true },
	{"|",             "pipe",        TOKEN_PIPE,            true },
	{"!",             "bang",        TOKEN_BANG,            true },
	{"&&",            "AND_IF",      TOKEN_AND_IF,          true },
	{"||",            "OR_IF",       TOKEN_OR_IF,           true },
	{";;",            "DSEMI",       TOKEN_DSEMI,           true },
	{"<<",            "DLESS",       TOKEN_DLESS,           true },
	{">>",            "DGREAT",      TOKEN_DGREAT,          true },
	{"<&",            "LESSAND",     TOKEN_LESSAND,         true },
	{"&>",            "GREATAND",    TOKEN_GREATAND,        true },
	{"<>",            "LESSGREAT",   TOKEN_LESSGREAT,       true },
	{"<<-",           "DLESSDASH",   TOKEN_DLESSDASH,       true },
	{">|",            "CLOBBER",     TOKEN_CLOBBER,         true },

	{"<",             "LESS",        TOKEN_LESS,            true },
	{">",             "GREAT",       TOKEN_GREAT,           true },
	{"~",             "TILDE",       TOKEN_TILDE,           false},

	{"^~/?[\\/\\w]*", "WORD",        TOKEN_WORD,            false},
	{"^\\D\\w+",      "NAME",        TOKEN_NAME,            false},
	{"\\d*[><]\\d*",  "IO_NUMBER",   TOKEN_IO_NUMBER,       false},
	{"^\\w+\\=",      "ASSIGNMENT",  TOKEN_ASSIGNMENT_WORD, false},

	{"\n",            "NEWLINE",     TOKEN_NEWLINE,         true },
	{"&",             "AMPERSAND",   TOKEN_AMPERSAND,       true },

	{NULL,            NULL,          TOKEN_KEYWORD,         false},
};

/*
** WORD, NAME and ASSIGNMENT_WORD regexes contain caret because these tokens
** must be a single string extracted from input and free from leading characters
*/

/*
** Returns true if char needs separate token
*/

int						is_single_token(char c)
{
	int		i;
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = 0;
	i = 0;
	while (g_singles[i])
		if (ft_strcmp(g_singles[i++], tmp) == 0)
			return (1);
	return (0);
}

/*
** There is some cringy stuff going on today
*/

int						count_substrings(char *str)
{
	ssize_t	i;
	int		subs;
	char	c;

	subs = 0;
	i = -1;
	while (str && str[++i])
	{
		c = str[i];
		if (ft_strchr(LIBFT_WHTSP, str[i]) != NULL && ++i)
			continue ;
		else if (is_single_token(str[i++]))
			subs++;
		else if (ISQT(str[i]) && ++i)
			while (str[i] && str[i] != c)
				i++;
		else if (ft_isdigit(str[i]))
			while (str[i] && ft_isdigit(str[i]) && !is_single_token(str[i]))
				i++;
		else
			while (str[i] && ft_strchr(TOKEN_DELIMITERS, str[i]) == NULL
				&& !is_single_token(str[i]))
				i++;
		subs++;
	}
	return (subs);
}

long long				get_word_size(char *str)
{
	long long	i;
	char		quote;

	quote = *str;
	i = 0;
	if (is_single_token(*str))
		return (1);
	else if (ISQT(*str) && ++i)
		while (str[i] && str[i] != quote)
			i++;
	else if (ft_isdigit(*str) && ++i)
		while (str[i] && ft_isdigit(str[i]))
			i++;
	else
		while (str[i] && !is_single_token(str[i]) && !ft_strchr(TOKEN_DELIMITERS, str[i]))
			i++;
	// TODO: Don't forget about this guy
	assert(i > 0);
	return (i);
}

/*
** Split that takes into account quotes ("", '', ``), separators - ';', '\n'
** and brackets
** TODO: Try to fix that too high memory allocation thing
*/
char					**smart_split(char *str, char *delimiters)
{
	char		**array;
	long long	j;
	long long	i;
	long long	word_size;
	long long	subs;

	array = (char **)ft_memalloc(sizeof(char *) *
				(subs = count_substrings(str) + 2));
	j = 0;
	i = -1;
	while (str[++i] && j < subs - 1)
	{
		if (ft_strchr(delimiters, str[i]) != NULL)
			continue ;
		word_size = get_word_size(str + i);
		array[j++] = ft_strsub(str + i, 0, word_size + ISQT(str[i]));
		if (str[i] != ';')
			i += word_size - !ISQT(str[i]);
	}
	array[j] = NULL;
	return (array);
}
