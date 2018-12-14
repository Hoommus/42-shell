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
	u_int64_t	key;
	void		(*handler) (u_int64_t);
};

extern struct s_listener	g_key_listeners[];

int							ft_putc(int c);
int							ft_strchr_back(const char *str, char c, int i);
void						buffer_clear(char symbol);
void						buffer_redraw(int symbol_index);

/*
** Key handlers (handlers_zbase.c, handlers_arrows.c, handlers_editing.c)
*/
void						handle_key(u_int64_t key);

void					handle_delchar(u_int64_t key);
void						handle_del(u_int64_t key);
void						handle_eot(u_int64_t key);
void						handle_up(u_int64_t key);
void						handle_down(u_int64_t key);
void						handle_left(u_int64_t key);
void						handle_right(u_int64_t key);
void						handle_ignore(u_int64_t key);
void						handle_line_kill(u_int64_t key);
void						handle_backspace(u_int64_t key);

/*
** State machine (state_machine.c)
*/
int							toggle_state(const char *c);
int							toggle_quote(void);
int							toggle_bquote(void);
int							toggle_dquote(void);
int							toggle_escaped(void);
/*
** Caret positions manipulation (cursor_positions.c)
*/
void						adjust_carpos_db(void);
int							caret_move(int distance,
										enum e_direction direction);
t_carpos					*carpos_get(enum e_position type);
t_carpos					*get_caretpos(enum e_position type);
t_carpos					*load_caret_position(enum e_position type);
t_carpos					*carpos_save_as(enum e_position type);
void						carpos_update(enum e_position type);


#endif
