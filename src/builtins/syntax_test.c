/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 18:28:22 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/14 17:07:44 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"

int				hs_syntax(const char **args)
{
	char	*string;
	bool	logging;

	logging = flag_short_present((const char **)args, 'l');
	if (logging)
		args++;
	TERM_APPLY_CONFIG(g_term->context_original->term_config);
	if (read_filename(args[0], &string))
	{
		TERM_APPLY_CONFIG(g_term->context_current->term_config);
		return (OK);
	}
	run_script(tokenize(string, TOKEN_DELIMITERS), logging);
	free(string);
	TERM_APPLY_CONFIG(g_term->context_current->term_config);
	return (0);
}
