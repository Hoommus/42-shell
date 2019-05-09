/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_engine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 11:54:55 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/04 15:49:40 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"

static const struct s_listener	g_key_hooks[] =
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
	{{K_DEL}, &handle_del},
	{{K_HOME}, &handle_home},
	{{K_END}, &handle_end},
	{{K_CTRL_D}, &handle_eot},
	{{K_CTRL_U}, &handle_ctrl_u},
	{{K_CTRL_Y}, &handle_ctrl_y},
	{{K_UP_FALLBACK}, &handle_up},
	{{K_DOWN_FALLBACK}, &handle_down},
	{{K_LEFT_FALLBACK}, &handle_left},
	{{K_RIGHT_FALLBACK}, &handle_right},
	{{CINTR}, &handle_tab},
	{{CSTATUS}, &handle_tab},
	{{CMIN}, &handle_tab},
	{{CSUSP}, &handle_tab},
	{{CTIME}, &handle_tab},
	{{CSTART}, &handle_tab},
	{{'\t'}, &handle_tab},
	{{0}, 0}
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
		TERM_INVERT;
		ft_printf("%c", '%');
		TERM_DISABLE_APPEARANCE;
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
			g_key_hooks[i].handler(key);
			break ;
		}
}
