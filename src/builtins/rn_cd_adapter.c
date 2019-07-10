/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_adapter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 21:43:46 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/09 22:02:56 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

const char	*get_env_var_ad(char *varname)
{
	t_var	*var;

	var = get_env_v(NULL, varname);
	if (var)
		return (var->value);
	return (NULL);
}

int			set_env_var_ad(char *varname, char *value)
{
	if (varname && value)
	{
		set_env_v(NULL, varname, value, SCOPE_EXPORT);
		return (1);
	}
	return (0);
}
