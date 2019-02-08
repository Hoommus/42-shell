/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 14:44:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/12 16:03:55 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

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

/*
** case syntax structure is avoided.
** But here its tokens are, just in case (no pun intended)
** {"case",          "case",        TOKEN_CASE,            false},
** {"esac",          "esac",        TOKEN_ESAC,            false},
** {";;",            "DSEMI",       TOKEN_DSEMI,           true },
*/

const struct s_parse_token	g_tokens[] = {
	{"if",            "IF",          TOKEN_IF,              false},
	{"then",          "THEN",        TOKEN_THEN,            false},
	{"else",          "ELSE",        TOKEN_ELSE,            false},
	{"elif",          "ELIF",        TOKEN_ELIF,            false},
	{"fi",            "FI",          TOKEN_FI,              false},
	{"do",            "DO",          TOKEN_DO,              false},
	{"done",          "DONE",        TOKEN_DONE,            false},
	{"while",         "WHILE",       TOKEN_WHILE,           false},
	{"until",         "UNTIL",       TOKEN_UNTIL,           false},
	{"for",           "FOR",         TOKEN_FOR,             false},

	{"in",            "IN",          TOKEN_IN,              false},

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
	{"|",             "PIPE",        TOKEN_PIPE,            true },
	{"<<-",           "DLESSDASH",   TOKEN_DLESSDASH,       true },
	{"<<",            "DLESS",       TOKEN_DLESS,           true },
	{">>",            "DGREAT",      TOKEN_DGREAT,          true },
	{"<&",            "LESSAND",     TOKEN_LESSAND,         true },
	{"&>",            "GREATAND",    TOKEN_GREATAND,        true },
	{"<>",            "LESSGREAT",   TOKEN_LESSGREAT,       true },
	{">|",            "CLOBBER",     TOKEN_CLOBBER,         true },

	{"<",             "LESS",        TOKEN_LESS,            false},
	{">",             "GREAT",       TOKEN_GREAT,           false},
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

/*
** WORD, NAME and ASSIGNMENT_WORD regexes contain caret because these tokens
** must be a single string extracted from input and free from leading characters
*/

/*
** Returns true if char needs separate token
*/

static int				is_single_token(char c)
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

static int				count_substrings(char *str)
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
		else
			while (str[i] && ft_strchr(TOKEN_DELIMITERS, str[i]) == NULL
				&& !is_single_token(str[i]))
				i++;
		subs++;
	}
	return (subs);
}

static long long		get_word_size(char *str)
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
 * TODO: Remove this shit
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
