/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acompl_alias.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 03:17:00 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/06 19:33:35 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "acompl.h"
#include "shell_environ.h"

extern t_env_vector	*g_alias_vector;
 
int					get_aliasv(char *input_str, char *aliasv[])
{
	size_t	input_len;
	size_t	aliasi;
	size_t	i;

	input_len = ft_strlen(input_str);
	aliasi = 0;
	i = 0;
	while (((t_var *)g_alias_vector->array)[i].key)
	{
		if (ft_strnstr(((t_var *)g_alias_vector->array)[i].key,
						input_str,
						input_len))
			aliasv[aliasi++] = ft_strdup(
					((t_var *)g_alias_vector->array)[i].key);
		i++;
	}
	aliasv[aliasi] = NULL;
	return (0);
}

int					get_aliasc(char *input_str)
{
	size_t	input_len;
	size_t	aliasc;
	size_t	i;

	input_len = ft_strlen(input_str);
	aliasc = 0;
	i = 0;
	while (((t_var *)g_alias_vector->array)[i].key)
	{
		if (ft_strnstr(((t_var *)g_alias_vector->array)[i].key,
						input_str, input_len))
			aliasc++;
		i++;
	}
	return (aliasc);
}
