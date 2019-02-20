/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_editing.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:56:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/18 18:40:27 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_EDITING_H
# define LINE_EDITING_H

# include "twenty_one_sh.h"

# define K_CTRL_W          23
# define K_CTRL_U          CKILL
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
**  if (ch == 127)
**		symbol_del(shell);
**	else if (ch >= 32 && ch < 127)
**		enter_ch(shell, ch);
**	else if (ch == 4283163)
**		history_up(shell);
**	else if (ch == 4348699)
**		history_down(shell);
**	else if (ch == 4414235)
**		right_key(shell);
**	else if (ch == 4479771)
**		left_key(shell);
**	else if (ch == 25115)
**		alt_left_key(shell);
**	else if (ch == 26139)
**		alt_right_key(shell);
**	else if (ch == 4741915)
**		home_key(shell);
**	else if (ch == 4610843)
**		end_key(shell);
**	else if (ch == 74982532143899)
**		shift_left(shell);
**	else if (ch == 73883020516123)
**		shift_right(shell);
**	else if (ch == 23)
**		cut(shell);
**	else if (ch == 27)
**		left_selection(shell);
**	else if (ch == 29)
**		right_selection(shell);
**	else if (ch == 5)
**		copy(shell);
**	else if (ch == 18)
**		paste(shell);
**	else if (ch == 4)
**		printf("CTRL+D\n");
**	else if (ch == 16)
**		play_music();
**	else if (ch == 12)
**		stop_music();
*/

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
	union u_char	key;
	void			(*handler) (union u_char);
};

int							ft_putc(int c);
void						buffer_redraw(void);

/*
** Key handlers (handlers_zbase.c, handlers_arrows.c, handlers_editing.c)
*/
void						handle_key(union u_char key);

void					handle_delchar(union u_char key);
void						handle_home(union u_char key);
void						handle_end(union u_char key);
void						handle_del(union u_char key);
void						handle_eot(union u_char key);

void						handle_ctrl_w(union u_char key);

void						handle_up(union u_char key);
void						handle_down(union u_char key);
void						handle_left(union u_char key);
void						handle_right(union u_char key);
void						handle_ignore(union u_char key);
void						handle_line_kill(union u_char key);
void						handle_backspace(union u_char key);
void						handle_alt_up(union u_char key);
void						handle_alt_down(union u_char key);
void						handle_alt_left(union u_char key);
void						handle_alt_right(union u_char key);
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
void						carpos_adjust_db(int by);
int							caret_move(int distance,
										enum e_direction direction);
t_carpos					*carpos_get(enum e_position type);
t_carpos					*get_caretpos(enum e_position type);
t_carpos					*carpos_load(enum e_position type);
t_carpos					*carpos_save_as(enum e_position type);
t_carpos					*carpos_update(enum e_position type);

void						write_at(int col, int row, char *string);

#endif
