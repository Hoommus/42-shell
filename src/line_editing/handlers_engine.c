/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_engine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 11:54:55 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 15:58:42 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"

static const struct s_listener	g_key_listeners[] =
{
	{{K_UP}, &handle_up},
	{{K_DOWN}, &handle_down},
	{{K_LEFT}, &handle_left},
	{{K_RIGHT}, &handle_right},
	{{K_ALT_UP}, &handle_alt_up},
	{{K_ALT_DOWN}, &handle_alt_down},
	{{K_ALT_LEFT}, &handle_alt_left},
	{{K_ALT_RIGHT}, &handle_alt_right},
	{{K_BSP}, &handle_backspace},
	{{K_DEL}, &handle_del},
	{{K_HOME}, &handle_home},
	{{K_END}, &handle_end},
	{{K_CTRL_D}, &handle_eot},
	{{K_CTRL_U}, &handle_ctrl_u},
	{{K_CTRL_Y}, &handle_ctrl_y},
	{{K_CTRL_W}, &handle_ctrl_w},
	{{'\t'}, &handle_tab},
	{{0}, 0}
};

bool							is_key_hooked(union u_char key)
{
	int		i;

	i = 0;
	while (g_key_listeners[i].handler)
		if (g_key_listeners[i++].key.lng == key.lng)
			return (true);
	return (false);
}

void							handle_tab(union u_char key)
{
	key.lng = 1337;
}

void							handle_eot(union u_char key)
{
	if (key.lng == CEOT && g_term->buffer->size == 0)
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
		ft_dprintf(2, SH ": warning: here-document delimited by end-of-file "
					"(wanted `%s')\n", g_term->heredoc_word);
		g_term->input_state = STATE_COMMIT;
	}
}

void							handle_delchar(union u_char key)
{
	key.lng = 0;
	tputs(tgetstr("dc", NULL), 1, &ft_putc);
}

void							handle_key(union u_char key)
{
	int		i;

	i = -1;
	while (g_key_listeners[++i].handler != 0)
		if (g_key_listeners[i].key.lng == key.lng)
			g_key_listeners[i].handler(key);
	write_at(0, 0, key.arr);
}
