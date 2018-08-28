#ifndef LINE_EDITING_H
# define LINE_EDITING_H

# include "twenty_one_sh.h"

# define K_UP     4283163
# define K_LEFT   4479771
# define K_RIGHT  4414235
# define K_DOWN   4348699
# define K_BSP    127
# define K_DEL    2117294875

typedef void					(*t_listener) (int);


/*
** Cursor movement direction
*/
enum							e_direction
{
	D_UP,
	D_DOWN,
	D_LEFT,
	D_RIGHT,
	D_NOWHERE
};

struct							s_listener_map
{
	int						key;
	t_listener				handler;
};

extern struct s_listener_map	g_key_listeners[];

void							handle_key(int key);
int								ft_putc(int c);
int								cursor_move(int distance,
											enum e_direction direction);
void							update_cursor_position(void);

void							handle_del(int key);
void							handle_eot(int key);
void							handle_left(int key);
void							handle_right(int key);
void							handle_line_kill(int key);
void							handle_backspace(int key);

int								delete_char_at(char *str, int64_t index);


#endif
