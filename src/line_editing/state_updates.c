/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_updates.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 20:16:53 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/28 13:36:05 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"

enum e_input_state	update_state(const char *input)
{
	const u_int64_t				iterator = g_term->buffer->iterator;
	const enum e_input_state	current = g_term->input_state;
	enum e_input_state			updated;
	char						*swap;

	if (current == STATE_HEREDOC || current == STATE_HEREDOCD)
		return (current);
	updated = current;
	if (!ft_strcmp(input, "\\") && buff_char_at_equals_any(iterator, "\"\\'`"))
	{
		swap = buff_char_at(iterator + 1);
		updated = toggle_state(swap);
	}
	else if (ft_strcmp(input, "\\") == 0 && current != STATE_QUOTE
		&& current != STATE_DQUOTE && current != STATE_BQUOTE)
		updated = toggle_escaped();
	else if (ft_strchr("\"\\'`", *input))
		updated = toggle_state(input);
	return (updated);
}
