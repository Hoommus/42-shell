/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 16:43:04 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/13 13:35:02 by vtarasiu         ###   ########.fr       */
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
	char		tmp[1024];
	t_var		*var;
	char		*swap;
	int			i;

	ft_bzero(tmp, sizeof(char) * 1024);
	if (*(str + *off + 1) == '$' || *(str + *off + 1) == '?' ||
		*(str + *off + 1) == '!' || *(str + *off + 1) == '0')
		return (extract_special(str, off));
	i = *off + 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	ft_memcpy((char *)tmp, str + *off + 1, i - *off - 1);
	if (is_valid_var(tmp))
	{
		var = environ_get_entry(g_term->context_current->environ, tmp);
		swap = strinsert(str, var ? var->value : "", *off, ft_strlen(tmp) + 1);
		*off += ft_strlen(var ? var->value : " ") - 1;
	}
	else
		swap = ft_strdup(str);
	if (!is_valid_var(tmp))
		*off += 1;
	return (swap);
}

#define NORMAL 0
#define DQUOTE 1
#define QUOTE 2

static int		check_ignore(const char *str, u_int32_t i, int ignore)
{
	if (i > 0 && str[i - 1] == '\\')
		return (ignore);
	if (str[i] == '"' && ((i > 0 && str[i - 1] != '\\') || i == 0))
		ignore = (ignore == DQUOTE) ? NORMAL : DQUOTE;
	else if (str[i] == '\'' && ((i > 0 && str[i - 1] != '\\') || i == 0))
		ignore = (ignore == NORMAL) ? QUOTE : ignore;
	return (ignore);
}

char			*expand_vars(char *str)
{
	u_int32_t	i;
	size_t		len;
	char		*swap;
	int			ignore;

	str = ft_strdup(str);
	ignore = NORMAL;
	i = 0;
	len = ft_strlen(str);
	while (i < len)
	{
		ignore = check_ignore(str, i, ignore);
		if (ignore != QUOTE && str[i] == '$' && i + 1 != len &&
			ft_isascii(str[i + 1]))
		{
			swap = str;
			str = extract_var(str, &i);
			ft_memdel((void **)&swap);
		}
		else
			i += (str[i] == '\\') ? 2 : 1;
	}
	return (str);
}
