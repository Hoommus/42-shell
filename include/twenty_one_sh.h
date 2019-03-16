/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   twenty_one_sh.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 18:12:03 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/16 15:13:59 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef TWENTY_ONE_SH_H

# define TWENTY_ONE_SH_H

# pragma clang diagnostic push
# pragma ide diagnostic ignored "readability-avoid-const-params-in-decls"

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
# include "shell_environ.h"

# define SNWH (copy[i + 1] == '/' || copy[i + 1] == 0 || ft_iswhsp(copy[i + 1]))
# define ABS(a) ((a) < 0 ? -(a) : (a))
# define PROMPT_HOST "\x1b[0m\x1b[34;1m[%s@%s]\x1b[0m"
# define PROMPT_PATH " \x1b[36;1m%s\x1b[0m"
# define PROMPT_TERMINATOR " \x1b[%d;1m$\x1b[0m "
# define SHELL_PROMPT PROMPT_HOST PROMPT_PATH PROMPT_TERMINATOR

// TODO: Replace all APPLY_CONFIGs with context switches
# define TERM_APPLY_CONFIG(term) tcsetattr(0, TCSANOW, term)

# define TERM_CLR_LINES_BELOW tputs(tgetstr("cd", NULL), 1, &ft_putc)
# define TERM_CLR_LINE tputs(tgetstr("ce", NULL), 1, &ft_putc)
# define TERM_CLR_CHAR tputs(tgetstr("dc", NULL), 1, &ft_putc)

# define HISTORY_FILE ".21sh_history"
# define CONFIG_FILE ".21shrc"
# define LOG_FILE ".21sh.log"

# define BUILD 1104
# define BUILD_DATE "16.03.19 15:13:59 EET"

# ifdef MAX_INPUT
#  undef MAX_INPUT
#  define MAX_INPUT 256
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

struct					s_fd_lst
{
	char				*label;
	short				original;
	short				current;
	struct s_fd_lst		*next;
};

/*
 * TODO: Add info about shell config
 */

typedef struct			s_context
{
	t_environ_vector	*environ;
	struct termios		*term_config;
	struct s_fd_lst		*fd_list;
}						t_context;

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

	short				history_file;
	short				logfile;

	short				flags;

	int					last_cmd_status;
	pid_t				running_process;

	struct s_context	*context_original;
	struct s_context	*context_current;
	struct s_context	*context_backup;

	t_buffer			*buffer;
};

extern struct s_term	*g_term;

int						execute(char **args);

/*
** Init (init.c)
*/
void					init_shell_context(void);
struct termios			*init_term(void);
void					init_files(void);
short					init_fd_at_home(char *filename, int flags);

/*
** Environment (environ_utils.c)
*/
t_var					*get_env_v(t_environ_vector *vector, const char *key);
int						set_env_v(t_environ_vector *vector, const char *key,
	const char *value, enum e_var_scope scope);
int						unset_env_v(t_environ_vector *vector, const char *key);

/*
** Context management
*/
void					context_switch(t_context *to_which);
t_context				*context_init(void);
t_context				*context_duplicate(const t_context *context,
	bool with_dup);
void					context_add_fd(t_context *context, const int original,
	const int actual, const char *label);
void					context_remove_fd(t_context *context, const int fd);
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
u_int64_t				hash_sdbm(const char *str);
ssize_t					ponies_teleported(void);
void					increment_shlvl(void);

/*
** Final input parsing (variables_replacement.c)
*/

char					*replace_variables(char *line);
char					*replace_home(char *line);
void					expand_variables(char **line);
int						is_valid_var(char *var);

/*
** Memory utils (memory.c)
*/

void					chfree(void *obj);
void					chfree_n(int n, ...);
void					free_array(void **array);

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
 * Syscall wrappers
 */
int						open_wrapper(const char *path, int oflag);
int						openm_wrapper(const char *path, int oflag, mode_t mode);
int						close_wrapper(int filedes);

/*
** Compatibility
*/
# ifdef __linux__

int						gethostname(char *arr, size_t size);

# endif

# pragma clang diagnostic pop

#endif

