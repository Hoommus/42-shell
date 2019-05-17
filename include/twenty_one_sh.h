/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   twenty_one_sh.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 18:12:03 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/16 13:05:17 by vtarasiu         ###   ########.fr       */
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
# include "shell_environ.h"

# ifndef SH
#  define SH "21sh"
# endif

# define ANSI_RESET "\x1b[0m"

# define PROMPT_HOST ANSI_RESET "\x1b[34;1m[%s@%s]\x1b[0m"
# define PROMPT_PATH " \x1b[36;1m%s\x1b[0m"
# define PROMPT_TERMINATOR "\x1b[%d;1m $ \x1b[0m"
# define SHELL_PROMPT PROMPT_HOST PROMPT_PATH PROMPT_TERMINATOR

# define TERM_APPLY_CONFIG(term) tcsetattr(0, TCSANOW, term)

# define TERM_CLR_LINES_BELOW tputs(tgetstr("cd", NULL), 1, &ft_putc)
# define TERM_CLR_LINE tputs(tgetstr("ce", NULL), 1, &ft_putc)
# define TERM_CLR_CHAR tputs(tgetstr("dc", NULL), 1, &ft_putc)
# define TERM_INVERT tputs(tgetstr("mr", NULL), 1, &ft_putc)
# define TERM_DISABLE_APPEARANCE tputs(tgetstr("me", NULL), 1, &ft_putc)

# define HISTORY_FILE "." SH "_history"
# define CONFIG_FILE "." SH "shrc"
# define LOG_FILE "." SH ".log"

# define SYNTAX_ERROR ": syntax error near token `%s' on line %d\n"

# define ERR_PERMISSION_DENIED  ANSI_RESET SH ": permission denied: %s\n"
# define ERR_COMMAND_NOT_FOUND  ANSI_RESET SH ": command not found: %s\n"
# define ERR_BAD_FD             ANSI_RESET SH ": Bad file descriptor: %d\n"
# define ERR_NO_SUCH_FILE      ANSI_RESET SH ": no such file or directory: %s\n"
# define ERR_IS_A_DIRECTORY     ANSI_RESET SH ": `%s': is a directory\n"
# define ERR_SYNTAX_AT_LINE     ANSI_RESET SH SYNTAX_ERROR
# define ERR_RUNNING_JOBS       ANSI_RESET SH ": you have running jobs\n"
# define ERR_AMBIGUOUS_REDIRECT ANSI_RESET SH ": ambiguous redirect\n"

# define BUILD 2459
# define BUILD_DATE "16.05.19 13:05:16 EEST"

# ifdef MAX_INPUT
#  undef MAX_INPUT
#  define MAX_INPUT 1024
# endif

# ifndef MAX_FD
#  define MAX_FD 4860
# endif

# define STATE_UNIVERSAL UINT32_MAX

enum					e_input_state
{
	STATE_NORMAL = 1,
	STATE_NON_INTERACTIVE = 2,
	STATE_QUOTE = 4,
	STATE_DQUOTE = 8,
	STATE_HEREDOC = 16,
	STATE_HEREDOCD = 32,
	STATE_ESCAPED = 64,
	STATE_EMPTY_OPERATOR = 128,
	STATE_PIPE_HEREDOC = 256,
	STATE_NEXT_ESCAPED = 512,
	STATE_COMMIT = 1024,
	STATE_SEARCH = 2048,
	STATE_PARTIAL_EXPAND = 4096,
	STATE_BQUOTE = 8192,
	STATE_JOB_IN_FG = 16384,
	STATE_EXPANSION = 32768,
	STATE_BREAK = 65536,
	STATE_LIMITED = 131072,
	STATE_VIM = 262144,
	STATE_EMACS = 524288,
};

struct					s_fd_lst
{
	int					original;
	int					current;
	char				*label;
	struct s_fd_lst		*next;
};

/*
** So context is an entity that controls used environment variables,
** term config and filedes table for easy duplications used in
** redirects and pipes.
**
** This thing is quite "heavy" regarding memory and resource usage
**
** TODO: Add info about shell config
*/

typedef struct			s_context
{
	t_env_vector		*environ;
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
} __attribute__((packed))						t_carpos;

/*
** g_term stores terminal parameters as well as cursor position and input buffer
** TODO: extract buffer variable to separate global var and create normal API
** TODO: consider adding errno-like global variable
*/

struct					s_term
{
	enum e_input_state	input_state;
	enum e_input_state	fallback_input_state;
	char				*heredoc_word;
	short				ws_col;
	short				ws_row;
	short				tty_fd;
	t_carpos			carpos_db[7];

	short				history_file;

	short				flags;

	int					last_status;
	pid_t				running_process;

	struct s_context	*context_original;
	struct s_context	*context_current;
	struct s_context	*context_backup;

	t_buffer			*buffer;
};
extern volatile sig_atomic_t	g_interrupt;
extern struct s_term			*g_term;

/*
** Init (init.c)
*/
struct termios			*init_term(void);
void					init_shell_context(void);
void					init_files(void);
void					init_variables(void);
short					init_fd_at_home(char *filename, int flags);
int						parse_args(int argc, char **argv);

/*
** Environment (environ_utils.c)
*/

t_var					*get_env_v(t_env_vector *vector, const char *key);
int						set_env_v(t_env_vector *vector, const char *key,
	const char *value, enum e_var_scope scope);
int						unset_env_v(t_env_vector *vector, const char *key);

/*
** Context management
*/
void					context_switch(t_context *to_which);
void					context_deep_free(t_context **context);
t_context				*context_duplicate(const t_context *context,
	bool with_dup);
void					context_add_fd(t_context *context, const int original,
	const int actual, const char *label);
void					context_remove_fd(t_context *context, const int fd);
void					context_remove_ofd(t_context *context,
	const int original);
bool					context_is_fd_present(const t_context *context,
	const int original);

/*
** Main Loop (main.c, )
*/

char					*read_arbitrary(void);
void					setup_signal_handlers(void);
void					display_prompt(enum e_input_state state);
int						display_normal_prompt(void);

/*
** Auxilia (auxilia.c)
*/
u_int64_t				hash_sdbm(const char *str);
ssize_t					ponies_teleported(void);
int						read_fd(const int fd, char **result);
bool					is_dir(const char *path);
bool					is_string_numeric(const char *str, const int base);

/*
** Final input parsing (variables_replacement.c)
*/

bool					is_valid_var(const char *var);

/*
** Memory utils (memory.c)
*/

void					chfree_n(int n, ...);
void					free_array(void **array);

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
bool					flag_plus_short_present(const char **args,
												const char flag);

/*
** Syscall wrappers
*/

int						open_wrapper(const char *path, int oflag);
int						openm_wrapper(const char *path, int oflag, mode_t mode);
int						close_wrapper(int filedes);

/*
** Errors
*/
int						puterr(const char *format, const char *cause);

/*
** Compatibility
*/
# ifdef __linux__

int						gethostname(char *arr, size_t size);

# endif

#endif
