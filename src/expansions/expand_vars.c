/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 16:43:04 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/03 18:57:44 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansions_internal.h"
#include "twenty_one_sh.h"

static char		*extract_special(const char *str, u_int32_t *off)
{
	char		*dummy;
	char		*swap;
	t_var		*var;

	dummy = ft_strnew(2);
	dummy[0] = *(str + *off + 1);
	dummy[1] = 0;
	var = environ_get_entry(g_term->context_current->environ, dummy);
	swap = strinsert(str, var ? var->value : "", *off, ft_strlen(dummy) + 1);
	*off += ft_strlen(var ? var->value : " ") - 1;
	free(dummy);
	return (swap);
}

static char		*extract_var(const char *str, u_int32_t *off)
{
	t_var		*var;
	char		**array;
	char		*swap;
	char		c;

	c = *(str + *off + 1);
	if (c == '$' || c == '?' || c == '!' || c == '0')
		return (extract_special(str, off));
	array = smart_split(str + *off + 1, "\n\t $&()*+,-./:;<=>[\\]^`{|}~\"\'");
	if (array && array[0] && is_valid_var(array[0]))
	{
		var = environ_get_entry(g_term->context_current->environ, array[0]);
		swap = strinsert(str, var ? var->value : "", *off,
			ft_strlen(array[0]) + 1);
		*off += ft_strlen(var ? var->value : " ") - 1;
	}
	else
		swap = ft_strdup(str);
	free_array((void **)array);
	return (swap);
}

char			*expand_vars(char *str)
{
	u_int32_t	i;
	size_t		len;
	char		*swap;
	bool		ignore;

	str = ft_strdup(str);
	ignore = false;
	i = 0;
	len = ft_strlen(str);
	while (i < len)
	{
		if (str[i] == '\'' && ((i > 0 && str[i - 1] != '\\') || i == 0))
			ignore = !ignore;
		if (!ignore && str[i] == '$' && i + 1 != len && ft_isascii(str[i + 1]))
		{
			swap = str;
			str = extract_var(str, &i);
			ft_memdel((void **)&swap);
			if ((len = ft_strlen(str)) == 0)
				break ;
		}
		else
			i += (str[i] == '\\') ? 2 : 1;
	}
	return (str);
}
