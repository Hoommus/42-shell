/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/16 13:48:01 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/16 13:48:01 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/twenty_one_sh.h"
#include "line_editing.h"

int		toggle_quote(void)
{
	if (g_term->input_state == STATE_QUOTE)
		return (g_term->input_state = STATE_NORMAL);
	else if (g_term->input_state == STATE_NORMAL)
		return (g_term->input_state = STATE_QUOTE);
	return (g_term->input_state);
}

int		toggle_dquote(void)
{
	if (g_term->input_state == STATE_DQUOTE)
		return (g_term->input_state = STATE_NORMAL);
	else if (g_term->input_state == STATE_NORMAL)
		return (g_term->input_state = STATE_DQUOTE);
	return (g_term->input_state);
}

int		toggle_bquote(void)
{
	if (g_term->input_state == STATE_BQUOTE)
		return (g_term->input_state = STATE_NORMAL);
	else if (g_term->input_state == STATE_NORMAL)
		return (g_term->input_state = STATE_BQUOTE);
	return (g_term->input_state);
}

int		toggle_escaped(void)
{
	if (g_term->input_state == STATE_ESCAPED)
		return (g_term->input_state = STATE_NORMAL);
	else if (g_term->input_state == STATE_NORMAL)
		return (g_term->input_state = STATE_ESCAPED);
	return (g_term->input_state);
}

int		toggle_state(const char *c)
{
	if (*c == '\'')
		return (toggle_quote());
	else if (*c == '"')
		return (toggle_dquote());
	else if (*c == '`')
		return (toggle_bquote());
	else if (*c == '\\')
		return (toggle_escaped());
	else
		return (g_term->input_state);
}
