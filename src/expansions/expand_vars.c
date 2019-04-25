/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 16:43:04 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/08 12:15:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansions_internal.h"
#include "twenty_one_sh.h"

// TODO: replace smart_split with function that returns first found token
// TODO: Make this one extract some tricky vars like `?', `@', `%', `$', etc.
static char		*extract_var(const char *str, u_int32_t *off)
{
	t_var		*var;
	char		**array;
	char		*swap;

	swap = ft_strdup(str);
	array = smart_split(str + *off + 1, "\n\t $!#%&()*+,-./:;<=>?@[\\]^`{|}~");
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

	str = ft_strdup(str);
	if (ft_strchr(str, '$') == NULL)
		return (str);
	i = 0;
	len = ft_strlen(str);
	while (i < len)
	{
		if (str[i] == '$' && i + 1 != len && ft_isascii(str[i + 1]))
		{
			swap = str;
			str = extract_var(str, &i);
			ft_memdel((void **)&swap);
			len = ft_strlen(str);
		}
		i += (str[i] == '\\') ? 2 : 1;
	}
	return (str);
}
