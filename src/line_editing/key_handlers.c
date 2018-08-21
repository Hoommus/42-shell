#include "../../include/line_editing.h"

struct s_listener_map	*g_key_listeners;

int						ft_putc(int c)
{
	return ((int) write(2, &c, 1));
}

void					register_key_listener(int key, void (*listener)(int))
{
	struct s_listener_map	*new;
	struct s_listener_map	*copy;

	new = (struct s_listener_map *)malloc(sizeof(struct s_listener_map));
	new->key = key;
	new->handler = listener;
	new->next = NULL;
	copy = g_key_listeners;
	if (copy == NULL)
		g_key_listeners = new;
	else
		while (copy != NULL)
		{
			if (copy->next == NULL)
			{
				copy->next = new;
				return ;
			}
			copy = copy->next;
		}
}

void					handle_key(int key)
{
	struct s_listener_map	*copy;

	copy = g_key_listeners;
	while (copy != NULL)
	{
		if (copy->key == key)
			return (copy->handler(key));
		copy = copy->next;
	}
}

void		hook_listeners(void)
{
	register_key_listener(K_LEFT, &handle_left);
	register_key_listener(K_RIGHT, &handle_right);
	register_key_listener(K_BSP, &handle_backspace);
}

