/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_toggles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/16 13:48:01 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/14 21:15:12 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"

enum e_input_state	toggle_quotes(enum e_input_state current, u_int64_t i)
{
	if (i > 0 && current != STATE_ESCAPED && buff_char_at_equals(i - 1, "\\"))
		return (current);
	if (current < STATE_QUOTE && buff_char_at_equals(i, "'"))
		return (STATE_QUOTE);
	else if (current < STATE_QUOTE && buff_char_at_equals(i, "\""))
		return (STATE_DQUOTE);
	else if (current == STATE_QUOTE && buff_char_at_equals(i, "'"))
		return (g_term->fallback_input_state);
	else if (current == STATE_DQUOTE && buff_char_at_equals(i, "\""))
		return (g_term->fallback_input_state);
	return (current);
}

enum e_input_state	recheck_state(u_int64_t from_index)
{
	register enum e_input_state	current;
	register u_int64_t			i;

	if (g_term->input_state == STATE_HEREDOC ||
		g_term->input_state == STATE_HEREDOCD)
		return (g_term->input_state);
	current = g_term->fallback_input_state;
	i = from_index;
	while (i < g_term->buffer->size)
	{
		if (current == STATE_EMPTY_OPERATOR
			&& !buff_char_at_equals_any(i, LIBFT_WHTSP))
			current = g_term->fallback_input_state;
		if (current < STATE_QUOTE && buff_char_at_equals(i, "\\"))
			current = STATE_ESCAPED;
		else if (current < STATE_QUOTE && buff_char_at_equals(i, "|") &&
			!buff_char_at_equals(i - 1, ">"))
			current = STATE_EMPTY_OPERATOR;
		else if (current == STATE_ESCAPED)
			current = g_term->fallback_input_state;
		else
			current = toggle_quotes(current, i);
		i++;
	}
	return (g_term->input_state = current);
}
