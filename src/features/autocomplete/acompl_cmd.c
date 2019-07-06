/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acompl_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 03:17:00 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/06 19:28:33 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "acompl.h"

#define ALLCMDC(cmd) (cmd[0] + cmd[1] + cmd[2])

static void	free_cmdv(char *cmdv[])
{
	size_t	i;

	i = 0;
	while (cmdv[i])
		free(cmdv[i++]);
	free(cmdv); 
}

static int	get_cmdv(char *input_str, char **cmdv[])
{
	size_t	cmdc[3];

	cmdc[0] = get_builtc(input_str);
	cmdc[1] = get_aliasc(input_str);
	cmdc[2] = get_hashc(input_str);
	if (!(*cmdv = (char **)malloc(sizeof(char **) * (ALLCMDC(cmdc) + 1))))
		return (-1);
	if (get_builtv(input_str, *cmdv)
			|| get_aliasv(input_str, *cmdv)
			|| get_hashv(input_str, *cmdv))
	{
		free_cmdv(*cmdv);
		return (-1);
	}
	return (0);
}

int			acompl_cmd(char *input_str, char *result_buf, size_t res_size)
{
	int		ret;
	char	**cmdv;

	if (get_cmdv(input_str, &cmdv))
		return (ACOMPL_ERROR);
	ret = filter_paths(cmdv, result_buf, res_size);
	free_cmdv(cmdv);	
	return (ret);
}
