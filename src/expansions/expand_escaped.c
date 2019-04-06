/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_escaped.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 16:40:46 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/06 17:46:37 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansions_internal.h"
#include "twenty_one_sh.h"

static const struct s_escape_map	g_escape_map[] =
{
	{'n', '\n'},
	{'t', '\t'},
	{'e', 0x1b},
	{0, 0}
};

static int							numstr_base(const char *string)
{
	if (*string == 'x' || *string == 'X')
		return (16);
	if (*string == '0' && (string[1] == 'x' || string[1] == 'X'))
		return (16);
	if (*string == '0' && (string[1] >= '0' && string[1] <= '7'))
		return (8);
	return (10);
}

static void							assign_escaped(char *str, u_int32_t i,
	size_t l)
{
	const struct s_escape_map	*map = g_escape_map;
	int64_t						escaped;
	size_t						size;
	int							base;

	base = numstr_base(str);
	if ((escaped = ft_atoi_base(str, base)) > 0 && escaped < 32)
	{
		size = ft_nbrlen_base(escaped, base)
				+ (str[0] == '0' || str[0] == 'x' || str[0] == 'X')
				+ (str[1] == 'x' || str[1] == 'X');
		ft_memmove(str + i, str + i + size - 1, l - i - size + 1);
		str[i] = escaped;
	}
	else
		while (map->what)
			if (str[i] == map->what)
			{
				str[i] = map->where;
				break ;
			}
			else
				map++;
}

char								*expand_escaped(char *string)
{
	size_t						len;
	u_int32_t					i;

	i = -1;
	len = ft_strlen(string);
	string = ft_strdup(string);
	while (++i < len && string[i])
		if (string[i] == '\\')
		{
			ft_memmove(string + i, string + i + 1, len - i + 1);
			assign_escaped(string, i, len);
		}
	return (string);
}
