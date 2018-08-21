#ifndef LINE_EDITING_H
# define LINE_EDITING_H

# include "twenty_one_sh.h"

# define K_UP     4283163
# define K_LEFT   4479771
# define K_RIGHT  4414235
# define K_DOWN   4348699
# define K_BSP    127
# define K_DEL    2117294875

# define ISCTRLSIG(x) (x == CEOF || x == CINTR || x == CKILL || x == CSTOP || x == CQUIT)

typedef void		(*t_listener) (int);


/*
** Cursor movement direction
*/
enum				e_direction
{
	D_UP,
	D_DOWN,
	D_LEFT,
	D_RIGHT,
	D_NOWHERE
};

struct				s_listener_map
{
	int						key;
	t_listener				handler;
	struct s_listener_map	*next;
};

extern struct s_listener_map	*g_key_listeners;

void							handle_key(int key);
int								ft_putc(int c);
int								cursor_move(int distance, enum e_direction direction);
void							register_key_listener(int key, void (*listener)(int));

void							handle_left(int key);
void							handle_right(int key);
void							handle_backspace(int key);

void							hook_listeners(void);

#endif
