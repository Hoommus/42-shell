/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rn_cd_is_logic.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 21:29:13 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/09 21:33:47 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

int	is_logic(char **argv[], bool *logic, const char *cmd)
{
	*logic = true;
	while (**argv)
	{
		if (!ft_strcmp(**argv, "-P"))
			*logic = false;
		else if (!ft_strcmp(**argv, "-L"))
			*logic = true;
		else if ((**argv)[0] == '-' && (**argv)[1] != '\0')
		{
			ft_dprintf(STDERR_FILENO,
					SH ": %s: %s: nema takogo praporu 🙄\n", cmd, **argv);
			return (-1);
		}
		else
			break ;
		(*argv)++;
	}
	return (0);
}
