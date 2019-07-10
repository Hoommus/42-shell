/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 03:17:00 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/10 13:27:00 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <locale.h>
#include "ft_printf.h"
#include "libft.h"
#include "acompl.h"

int	main(int argc, char *argv[])
{
	char	buf[PATH_MAX];
	int		ret;

	setlocale(LC_ALL, "");
	if (argc < 2)
		return (1);
	ret = acompl(argv[1], buf, PATH_MAX, true);
	if (ret == ACOMPL_END || ret == ACOMPL_CONTINUE)
		ft_printf("[%s] %lc %c", buf, ((ret == ACOMPL_END) ? L'🌝' : '\0'),
				((argc > 2) ? ' ' : '\n'));
	else
		ft_printf("[]%c", ((argc > 2) ? ' ' : '\n'));
	if (argc < 3)
		return (0);
	ret = acompl(argv[2], buf, PATH_MAX, false);
	if (argc == 3 && (ret == ACOMPL_END || ret == ACOMPL_CONTINUE))
		ft_printf("[%s] %lc\n", buf, ((ret == ACOMPL_END) ? L'🌝' : '\0'));
	else
		ft_printf("[]\n");
	return (0);
}
