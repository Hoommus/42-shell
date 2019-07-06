/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acompl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 03:17:00 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/03 17:00:49 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "acompl.h"

int	acompl(char *input_str, char *result_buf, size_t res_size, bool is_cmd)
{
	int	ret;

	if (is_cmd)
	{
		if ((ret = acompl_cmd(input_str,
				result_buf, res_size) != ACOMPL_NOTHING))
			return (ret);
		else
			return (acompl_file(input_str, result_buf, res_size, true));			
	}
	else
		return(acompl_file(input_str, result_buf, res_size, false));
}
