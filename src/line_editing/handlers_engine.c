/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_engine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 11:54:55 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/14 13:58:10 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"

static const struct s_listener	g_key_hooks[] =
{
	{{K_UP}, &handle_up, STATE_NORMAL | STATE_NON_INTERACTIVE},
	{{K_DOWN}, &handle_down, STATE_NORMAL | STATE_NON_INTERACTIVE},
	{{K_LEFT}, &handle_left, STATE_UNIVERSAL},
	{{K_RIGHT}, &handle_right, STATE_UNIVERSAL},
	{{K_ALT_UP}, &handle_alt_up, STATE_NORMAL | STATE_NON_INTERACTIVE},
	{{K_ALT_DOWN}, &handle_alt_down, STATE_NORMAL | STATE_NON_INTERACTIVE},
	{{K_ALT_LEFT}, &handle_alt_left, STATE_NORMAL | STATE_NON_INTERACTIVE},
	{{K_ALT_RIGHT}, &handle_alt_right, STATE_NORMAL | STATE_NON_INTERACTIVE},
	{{K_BSP}, &handle_backspace, STATE_UNIVERSAL},
	{{K_DEL}, &handle_del, STATE_UNIVERSAL},
	{{K_HOME}, &handle_home, STATE_UNIVERSAL},
	{{K_END}, &handle_end, STATE_UNIVERSAL},
	{{K_CTRL_D}, &handle_eot, STATE_UNIVERSAL},
	{{K_CTRL_U}, &handle_ctrl_u, STATE_NORMAL | STATE_NON_INTERACTIVE},
	{{K_CTRL_Y}, &handle_ctrl_y, STATE_NORMAL | STATE_NON_INTERACTIVE},
	{{K_UP_FALLBACK}, &handle_up, STATE_NORMAL | STATE_NON_INTERACTIVE},
	{{K_DOWN_FALLBACK}, &handle_down, STATE_NORMAL | STATE_NON_INTERACTIVE},
	{{K_LEFT_FALLBACK}, &handle_left, STATE_UNIVERSAL},
	{{K_RIGHT_FALLBACK}, &handle_right, STATE_UNIVERSAL},
	{{CINTR}, &handle_tab, STATE_UNIVERSAL},
	{{CSTATUS}, &handle_tab, STATE_UNIVERSAL},
	{{CMIN}, &handle_tab, STATE_UNIVERSAL},
	{{CSUSP}, &handle_tab, STATE_UNIVERSAL},
	{{CTIME}, &handle_tab, STATE_UNIVERSAL},
	{{CSTART}, &handle_tab, STATE_UNIVERSAL},
	{{'\t'}, &handle_tab, STATE_UNIVERSAL},
	{{0}, 0, 0}
};

bool							is_key_hooked(union u_char key)
{
	int		i;

	if (key.lng < 32 && key.lng != '\n')
		return (true);
	i = 0;
	while (g_key_hooks[i].handler)
		if (g_key_hooks[i++].key.lng == key.lng)
			return (true);
	return (false);
}

void							handle_tab(union u_char key)
{
	key.lng = 1337;
}

void							handle_eot(union u_char key)
{
	if (key.lng == CEOT && g_term->buffer->size == 0
						&& g_term->input_state != STATE_HEREDOC)
	{
		if (g_term->running_process != 0)
		{
			ft_dprintf(2, ERR_RUNNING_JOBS);
			display_normal_prompt();
		}
		else
			exit(ft_printf("exit\n") & 0);
	}
	else if (key.lng == CEOF && g_term->input_state == STATE_HEREDOC)
	{
		ft_dprintf(2, "\n" SH ": warning: here-document delimited "
			"by end-of-file (wanted `%s')\n", g_term->heredoc_word);
		g_term->input_state = STATE_COMMIT;
	}
}

void							handle_key(union u_char key)
{
	int		i;

	i = -1;
	while (g_key_hooks[++i].handler != 0)
		if (g_key_hooks[i].key.lng == key.lng && g_key_hooks[i].handler)
		{
			if ((g_key_hooks[i].states & g_term->input_state) != 0)
				g_key_hooks[i].handler(key);
			break ;
		}
}
