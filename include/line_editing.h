#ifndef LINE_EDITING_H
# define LINE_EDITING_H

# include "twenty_one_sh.h"

# define K_UP     4283163
# define K_LEFT   4479771
# define K_RIGHT  4414235
# define K_DOWN   4348699
# define K_BSP    127
# define K_DEL    2117294875

# define INITIAL_HISTORY_SIZE 256

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

struct						s_history_entry
{
	char		*command;
	time_t		timestamp;
};

typedef struct				s_history_vector
{
	struct s_history_entry	**entries;
	uint64_t				size;
	uint64_t				capacity;
}							t_history;

extern struct s_listener	g_key_listeners[];

int							ft_putc(int c);
int							delete_char_at(char *str, int64_t index);
int							ft_strchr_back(const char *str, char c, int i);

void						clear_buffer(char symbol);
void redraw_buffer(int symbol_index);

/*
** Key handlers (handlers_zbase.c, handlers_arrows.c, handlers_editing.c)
*/
void						handle_key(int key);

void				handle_delchar(int key);
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
** State machine (state_machine.c)
*/
void						toggle_quote(void);
void						toggle_bquote(void);
void						toggle_dquote(void);
void						toggle_escaped(void);

/*
** Caret positions manipulation (cursor_positions.c)
*/
void						adjust_carpos_db(void);
int							caret_move(int distance,
										enum e_direction direction);
t_carpos					*get_carpos(enum e_position type);
t_carpos					*get_caretpos(enum e_position type);
t_carpos					*load_caret_position(enum e_position type);
t_carpos					*save_caret_position_as(enum e_position type);
void						update_caret_position(enum e_position type);

/*
** History utilities (features/history.c, features/history_vector.c)
*/

void						init_vector(uint64_t capacity);
void						push_history_entry(const char *cmd, time_t timestamp);
void						save_history_entry(int fd);
void						load_history(int fd);
struct s_history_entry		*pop_entry(void);
struct s_history_entry		*get_entry(uint64_t index);

#endif
