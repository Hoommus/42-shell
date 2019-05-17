/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_engine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 18:59:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/11 21:00:11 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansions_internal.h"
#include "twenty_one_sh.h"

/*
** Leave .starting_point empty to enforce running such expansion on any string
*/

static const struct s_expansion		g_expansions[] =
{
	{"", expand_tilde},
	{"", expand_vars},
	{"", expand_escaped},
	{"", expand_quotes},
	{NULL, NULL},
	{"$(", NULL},
	{"${", NULL},
};

char								*expand_tilde(char *str)
{
	u_int32_t	i;
	t_var		*var;
	char		*swap;
	size_t		len;

	i = -1;
	str = ft_strdup(str);
	len = ft_strlen(str);
	var = environ_get_entry(g_term->context_current->environ, "HOME");
	while (++i < len)
	{
		if ((i == 0 || ft_strchr("\n\t ", str[i - 1]))
			&& str[i] == '~'
			&& (ft_strchr("\n\t /", str[i + 1]) || str[i + 1] == '\0'))
		{
			swap = strinsert(str, var ? var->value : "~", 0, 1);
			ft_memdel((void **)&str);
			str = swap;
			len = ft_strlen(str);
		}
	}
	return (str);
}

char								*expand(char *string)
{
	char	*swap;
	char	*final;
	int		i;
	int		j;

	final = ft_strdup(string);
	i = -1;
	while (g_expansions[++i].starting_point)
	{
		j = -1;
		while (++j != -42)
			if (g_expansions[i].starting_point[j] == '\0')
			{
				swap = g_expansions[i].function(final);
				ft_memdel((void **)&final);
				final = swap;
				break ;
			}
			else if (g_expansions[i].starting_point[j] != final[j])
				break ;
	}
	return (final);
}
