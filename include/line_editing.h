/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_editing.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:56:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/14 13:48:57 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_EDITING_H
# define LINE_EDITING_H

# include "twenty_one_sh.h"

# define K_CTRL_W          23
# define K_CTRL_U          21
# define K_CTRL_Y          25
# define K_CTRL_D          CEOT
# define K_BSP             127

# define K_DEL             2117294875
# define K_SHIFT_TAB       5921563

# define K_HOME            4741915
# define K_END             4610843

# define K_UP              4283163
# define K_DOWN            4348699
# define K_LEFT            4479771
# define K_RIGHT           4414235

# define K_UP_FALLBACK     4280091
# define K_DOWN_FALLBACK   4345627
# define K_LEFT_FALLBACK   4476699
# define K_RIGHT_FALLBACK  4411163

# define K_ALT_UP          1096489755
# define K_ALT_DOWN        1113266971
# define K_ALT_LEFT        1146821403
# define K_ALT_RIGHT       1130044187

# define K_SHIFT_UP        71683997260571
# define K_SHIFT_DOWN      72783508888347
# define K_SHIFT_LEFT      74982532143899
# define K_SHIFT_RIGHT     73883020516123

# define K_ALT_SHIFT_UP    18348861490813723
# define K_ALT_SHIFT_DOWN  18630336467524379
# define K_ALT_SHIFT_LEFT  19193286420945691
# define K_ALT_SHIFT_RIGHT 18911811444235035

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

/*
** .states field is a bitwise-OR separated value of states in which
** this specific listener will be executed.
*/

struct				s_listener
{
	const union u_char	key;
	void				(*const handler) (union u_char);
	enum e_input_state	states;
};

int					ft_putc(int c);
void				buffer_redraw(void);
void				buffer_redraw_i(u_int64_t from_index);
bool				is_key_hooked(union u_char key);

/*
** Key handlers (handlers_engine.c, handlers_arrows.c, handlers_editing.c)
*/
void				handle_key(union u_char key);

void				handle_home(union u_char key);
void				handle_end(union u_char key);
void				handle_del(union u_char key);
void				handle_eot(union u_char key);
void				handle_up(union u_char key);
void				handle_down(union u_char key);
void				handle_left(union u_char key);
void				handle_right(union u_char key);
void				handle_tab(union u_char key);
void				handle_ctrl_w(union u_char key);
void				handle_ctrl_u(union u_char key);
void				handle_ctrl_y(union u_char key);
void				handle_backspace(union u_char key);
void				handle_alt_up(union u_char key);
void				handle_alt_down(union u_char key);
void				handle_alt_left(union u_char key);
void				handle_alt_right(union u_char key);

/*
** State machine (state_machine.c, state_updates.c)
*/

enum e_input_state	toggle_quotes(enum e_input_state current, u_int64_t i);
enum e_input_state	recheck_state(u_int64_t from_index);

/*
** Caret positions manipulation (cursor_positions.c)
*/

void				carpos_adjust_db(int by);
t_carpos			caret_move(int distance, enum e_direction direction);
t_carpos			*carpos_get(enum e_position type);
t_carpos			*carpos_load(enum e_position type);
t_carpos			*carpos_save_as(enum e_position type);
t_carpos			*carpos_update(enum e_position type);

/*
** Auxiliary (auxiliary_le.c)
*/

void				write_at(int col, int row, const char *string);
bool				is_single_symbol(const char *c);

#endif
