#include "../../include/line_editing.h"

struct s_listener_map	g_key_listeners[] =
{
	{K_LEFT, &handle_left},
	{K_RIGHT, &handle_right},
	{K_BSP, &handle_backspace},
	{K_DEL, &handle_del},
	{CEOT, &handle_eot},
	{CKILL, &handle_line_kill},

	{0, 0}
};

int						ft_putc(int c)
{
	return ((int)write(2, &c, 1));
}

void	handle_eot(int key)
{
	if (key == CEOT && g_running_process == 0 && ft_strlen(g_term->buffer) == 0)
	{
		ft_printf("exit\n");
		exit(0);
	}
}

void					handle_key(int key)
{
	int		i;

	i = -1;
	while (g_key_listeners[++i].handler != 0)
		if (g_key_listeners[i].key == key)
			g_key_listeners[i].handler(key);
}
