#ifndef LINE_EDITING_H
# define LINE_EDITING_H

# include "twenty_one_sh.h"

# define K_UP     4283163
# define K_LEFT   4479771
# define K_RIGHT  4414235
# define K_DOWN   4348699
# define K_BSP    127
# define K_DEL    2117294875

/*
** Cursor movement direction
*/
enum						e_direction
{
	D_UP,
	D_DOWN,
	D_LEFT,
	D_RIGHT,
	D_NOWHERE
};

struct						s_listener
{
	int			key;
	void		(*handler) (int);
};

extern struct s_listener	g_key_listeners[];

int							ft_putc(int c);
void						clear_buffer(char symbol);
int							delete_char_at(char *str, int64_t index);
int							ft_strchr_back(const char *str, char c, int i);

void						redraw_buffer(void);

/*
** Key handlers
*/
void						handle_key(int key);

void						handle_del(int key);
void						handle_eot(int key);
void						handle_up(int key);
void						handle_down(int key);
void						handle_left(int key);
void						handle_right(int key);
void						handle_ignore(int key);
void						handle_line_kill(int key);
void						handle_backspace(int key);
/*
** State machine
*/
void						toggle_quote(void);
void						toggle_bquote(void);
void						toggle_dquote(void);
void						toggle_escaped(void);

/*
** Caret positions manipulation (cursor_positions.c)
*/
int							caret_move(int distance,
										enum e_direction direction);
t_carpos					*get_carpos(enum e_position type);
t_carpos					*get_caretpos(enum e_position type);
t_carpos					*load_caret_position(enum e_position type);
t_carpos					*save_caret_position_as(enum e_position type);
void						update_caret_position(enum e_position type);

#endif
