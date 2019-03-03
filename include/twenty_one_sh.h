/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   twenty_one_sh.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 18:12:03 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/03 18:04:20 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TWENTY_ONE_SH_H
# define TWENTY_ONE_SH_H

# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/ioctl.h>

# include <curses.h>
# include <term.h>
# include <termios.h>
# include <termcap.h>
# include <limits.h>
# include <sys/termios.h>

# include "libft.h"
# include "ft_printf.h"
# include "buffer_works.h"
# include "get_next_line.h"

# define SNWH (copy[i + 1] == '/' || copy[i + 1] == 0 || ft_iswhsp(copy[i + 1]))
# define ABS(a) ((a) < 0 ? -(a) : (a))
# define SHELL_PROMPT "\x1b[0m\x1b[34;1m[%s@%s] \x1b[36;1m%s\x1b[0m \x1b[%d;1m$\x1b[0m "

# define TERM_RESTORE tcsetattr(g_term->tty_fd, TCSANOW, g_term->original_term)
# define TERM_ENFORCE tcsetattr(g_term->tty_fd, TCSANOW, g_term->current_term)

# define TERM_CLR_LINES_BELOW tputs(tgetstr("cd", NULL), 1, &ft_putc)
# define TERM_CLR_LINE tputs(tgetstr("ce", NULL), 1, &ft_putc)
# define TERM_CLR_CHAR tputs(tgetstr("dc", NULL), 1, &ft_putc)

# define HISTORY_FILE ".21sh_history"
# define CONFIG_FILE ".21shrc"
# define LOG_FILE ".21sh.log"

# define BUILD 986
# define BUILD_DATE "03.03.19 18:04:20 EET"

/*
** Initial input of 260 is chosen because (260 * 10) % 8 == 0
*/

# ifdef MAX_INPUT
#  undef MAX_INPUT
#  define MAX_INPUT 260
# endif

/*
** ◦ pipe
** ◦ dup, dup2
** ◦ isatty, ttyname, ttyslot
** ◦ tgetent
** ◦ tgetflag
** ◦ tgetnum
*/

/*
** Used for controlling input state
*/

enum					e_input_state
{
	STATE_NORMAL,
	STATE_QUOTE,
	STATE_DQUOTE,
	STATE_BQUOTE,
	STATE_HEREDOC,
	STATE_ESCAPED_EOL,
	STATE_EMPTY_PIPE,
	STATE_PIPE_HEREDOC,
	STATE_NEXT_ESCAPED,
	STATE_COMMIT,
	STATE_SEARCH,
	STATE_PARTIAL_EXPAND,
	STATE_NON_INTERACTIVE,
	STATE_JOB_IN_FG,
	BREAK
};

/*
** Used to store specific caret positions. Heavy usage in cursor_positions.c
** Also, see line_editing.h
*/
enum					e_position
{
	POS_CURRENT,
	POS_PROMPT,
	POS_LAST,
	POS_TAIL,
	POS_CUSTOM1,
	POS_CUSTOM2,
	POS_ORIGIN
};

/*
** CArPOS. Got it? Like Carbon-Argentum-Phosphorus-Oxygen-Sulfur
** Alright, it's carpos for 'caret position'
*/
typedef struct			s_position
{
	short				col;
	short				row;
}						t_carpos;

/*
** g_term stores terminal parameters as well as cursor position and input buffer
** TODO: extract buffer variable to separate global var and create normal API
*/
struct					s_term
{
	enum e_input_state	input_state;
	short				ws_col;
	short				ws_row;
	short				tty_fd;
	t_carpos			carpos_db[7];
	struct termios		*original_term;
	struct termios		*current_term;

	short				history_file;
	short				logfile;

	short				flags;

	int					last_cmd_status;
	pid_t				running_process;

	t_buffer			*buffer;
};

char					**g_environ;
extern struct s_term	*g_term;

/*
** What is it? A design pattern? Really???
*/

int						execute(char **args);

/*
** Init (init.c)
*/
void					init_term(void);
void					init_files(void);
short					init_fd_at_home(char *filename, int flags);

/*
** Environment (environ_utils.c)
*/

char					*get_env(char *name);
int						set_env(char *key, char *value);
int						unset_env(char *name);
char					**copy_env(char **argenv, char **environ);

/*
** Main Loop (main.c, )
*/

char					**read_command(void);
int						shell_loop(void);
void					setup_signal_handlers(void);
void					display_prompt(enum e_input_state state);
int						display_normal_prompt(void);

/*
** Auxilia (auxilia.c)
*/

ssize_t					ponies_teleported(void);
void					increment_shlvl(void);

/*
** Final input parsing (variables_replacement.c)
*/

char					*replace_variables(char *line);
char					*replace_home(char *line);
void					restore_variables(void);
void					expand_variables(char **line);
int						is_valid_var(char *var);

/*
** Memory utils (memory.c)
*/

void					chfree(void *obj);
void					chfree_n(int n, ...);
void					free_array(void **array);

void					history_load(int fd);

/*
** errors.c
*/

void					throw_fatal(char *cause);

/*
** service_routines.c
*/

int						get_history_fd(void);

/*
** Arguments parsing
*/

bool					flag_long_present(const char **args, const char *flag);
bool					flag_short_present(const char **args, const char flag);
char					flag_validate_short(const char **args,
											const char *possible_flags);
/*
** Compatibility
*/
# ifdef __linux__

int						gethostname(char *arr, size_t size);

# endif

#endif
