/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_engine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 18:59:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/30 17:43:17 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "twenty_one_sh.h"
#include "shell_environ.h"

struct						s_expansion
{
	const char		*const starting_point;
	char			*(*const function)(char *string);
};

char						*strinsert(char *dst, const char *insertion,
	size_t region_start, size_t region_size)
{
	char	*final;

	if (!dst || !insertion)
		return (dst);
	final = ft_strnew(ft_strlen(dst) + ft_strlen(insertion) - region_size);
	ft_strncat(final, dst, region_start);
	ft_strcat(final + region_start, insertion);
	ft_strcat(final + region_start + ft_strlen(insertion),
				dst + region_start + region_size);
	return (final);
}

char						*expand_quote(char *string)
{
	u_int32_t	i;
	const char	q = '\'';

	string = ft_strdup(string);
	i = 0;
	while (string[i] && string[i] != q)
		i++;
	ft_memmove(string + i, string + i + 1, ft_strlen(string + i) + 1);
	while (string[i] && string[i] != q)
		i++;
	ft_memmove(string + i, string + i + 1, ft_strlen(string + i) + 1 + 1);
	return (string);

}

// TODO: expand everything inside
char						*expand_dquote(char *string)
{
	u_int32_t	i;
	const char	q = '\"';

	string = ft_strdup(string);
	i = 0;
	while (string[i] && string[i] != q)
		i++;
	ft_memmove(string + i, string + i + 1, ft_strlen(string + i) + 1);
	while (string[i] && string[i] != q)
		i++;
	ft_memmove(string + i, string + i + 1, ft_strlen(string + i) + 1 + 1);
	return (string);
}

char						*expand_tilde(char *string)
{
	t_var	*var;
	char	*swap;

	if (string && (string[1] == '/' || string[1] == '\0'))
	{
		var = environ_get_entry(g_term->context_current->environ, "HOME");
		swap = strinsert(string, var ? var->value : "", 0, 1);
		string = swap;
	}
	return (string);
}

/*
** TODO: Make this function expand concatenated variables like $HOME$PATH
*/
char						*expand_var(char *string)
{
	u_int32_t	i;
	t_var		*var;
	char 		**array;
	char		*swap;
	char		*final;

	final = ft_strnew(0);
	i = -1;
	while (string[++i])
		if (string[i] == '$' && ft_isascii(string[i + 1])
			&& (i == 0 || ft_strchr("=", string[i - 1])))
		{
			array = smart_split(string + i + 1, LIBFT_WHTSP);
			if (is_valid_var(array[0]))
			{
				var = environ_get_entry(g_term->context_current->environ, array[0]);
				swap = var ? var->value : "";
				ft_memdel((void **)&final);
				final = strinsert(string, swap, i, ft_strlen(array[0]) + 1);
				string = final;
				i += ft_strlen(array[0]);
			}
			free_array((void **)array);
		}
	return (string);
}

static struct s_expansion	g_expansions[] =
{
	{"$", expand_var},
	{"~", expand_tilde},
	{"", expand_quote},
	{"", expand_dquote},
	{NULL, NULL},
	{"$(", NULL},
	{"${", NULL},
};

// TODO: allocate big space in advance and reallocate if needed
char						*expand(char *string)
{
	char	*swap;
	int		i;
	int		j;

	string = ft_strdup(string);
	i = -1;
	while (g_expansions[++i].starting_point)
	{
		j = -1;
		while (++j != -42)
			if (g_expansions[i].starting_point[j] == '\0')
			{
				// TODO: Fix leaks if there are any
				swap = g_expansions[i].function(string);
				ft_memdel((void **)&string);
				string = swap;
				break ;
			}
			else if (g_expansions[i].starting_point[j] != string[j])
				break ;
	}
	return (string);
}

