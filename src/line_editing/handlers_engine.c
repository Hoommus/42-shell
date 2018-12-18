#include "line_editing.h"

static struct s_listener	g_key_listeners[] =
{
	{{K_UP}, &handle_up},
	{{K_DOWN}, &handle_down},
	{{K_LEFT}, &handle_left},
	{{K_RIGHT}, &handle_right},
	{{K_BSP}, &handle_backspace},
	{{K_DEL}, &handle_del},
	{{CEOT}, &handle_eot},
	{{CKILL}, &handle_line_kill},
	{{'\t'}, &handle_ignore},
//	{9, &handle_delchar},
	{{0}, 0}
};

int							ft_putc(int c)
{
	return ((int)write(2, &c, 1));
}

void						handle_ignore(union u_char key)
{
	key.lng = 1337;
}

void						handle_eot(union u_char key)
{
	if (key.lng == CEOT && g_term->running_process == 0 && g_term->buffer->size == 0)
	{
		ft_printf("exit\n");
		exit(0);
	}
}

void						handle_delchar(union u_char key)
{
	key.lng = 0;
	tputs(tgetstr("dc", NULL), 1, &ft_putc);
}

void						handle_key(union u_char key)
{
	int		i;

	i = -1;
	while (g_key_listeners[++i].handler != 0)
		if (g_key_listeners[i].key.lng == key.lng)
			g_key_listeners[i].handler(key);
	write_at(0, 0, key.arr);
}
