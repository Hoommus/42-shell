/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 16:43:04 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/06 17:17:16 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansions_internal.h"
#include "twenty_one_sh.h"

static char		*extract_var(const char *str, u_int32_t *off)
{
	t_var		*var;
	char		**array;
	char		*swap;

	swap = ft_strdup(str);
	array = smart_split(str + *off + 1, "\n\t $!#%&()*+,-./:;<=>?@[\\]^_`{|}~");
	if (array && array[0] && is_valid_var(array[0]))
	{
		var = environ_get_entry(g_term->context_current->environ, array[0]);
		ft_memdel((void **)&swap);
		swap = strinsert(str, var ? var->value : "", *off,
			ft_strlen(array[0]) + 1);
		*off += ft_strlen(var ? var->value : "") - 1;
	}
	free_array((void **)array);
	return (swap);
}

char			*expand_vars(char *str)
{
	u_int32_t	i;
	size_t		len;
	char		*swap;

	if (ft_strchr(str, '$') == NULL)
		return (ft_strdup(str));
	i = 0;
	swap = ft_strnew(0);
	len = ft_strlen(str);
	while (i < len)
	{
		if (str[i] == '$' && i + 1 != len && ft_isascii(str[i + 1]))
		{
			ft_memdel((void **)&swap);
			swap = extract_var(str, &i);
			str = swap;
			len = ft_strlen(str);
		}
		i += (str[i] == '\\') ? 2 : 1;
	}
	return (str);
}
