/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 18:28:22 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/22 17:20:41 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"

int				hs_syntax(char **args)
{
	char	*string;

	TERM_RESTORE;
	if (read_filename(args[0], &string))
	{
		TERM_ENFORCE;
		return (OK);
	}
	run_script(tokenize(string, TOKEN_DELIMITERS));
	TERM_ENFORCE;
	return (0);
}
