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

# include <term.h>
# include <termios.h>
# include <termcap.h>
# include <limits.h>
# include <curses.h>
# include <sys/termios.h>

# include "../printf/include/ft_printf.h"
# include "../libft/libft.h"
# include "../libft/get_next_line.h"

# define SNWH (copy[i + 1] == '/' || copy[i + 1] == 0 || ft_iswhsp(copy[i + 1]))
# define ABS(a) ((a) < 0 ? -(a) : (a))
# define SHELL_PROMPT "\x1b[0m\x1b[38;5;2m[%s@%s] \x1b[37;1m%s $> \x1b[0m"

# define BUILD 60
# define BUILD_DATE "07.09.18 16:41:21 EEST"

# undef MAX_INPUT
# define MAX_INPUT 2048

/*
** ◦ pipe
** ◦ dup, dup2
** ◦ isatty, ttyname, ttyslot
** ◦ tgetent
** ◦ tgetflag
** ◦ tgetnum
*/

// TODO: Remove after AST implemented
struct			command {
	char	*command;
	char	**args;
};

/*
** Real... e_state
** I mean, used for controlling input state
*/

enum				e_state
{
	NORMAL,
	QUOTE,
	DQUOTE,
	BQUOTE,
	HEREDOC,
	ESCAPED_NL,
	NEXT_ESCAPED,
	COMMIT,
	BREAK
};

/*
** Used to store specific caret positions. Heavy usage in cursor_positions.c
** Also, see line_editing.h
*/
enum				e_position
{
	POS_CURRENT,
	POS_PROMPT,
	POS_LAST,
	POS_CUSTOM1,
	POS_CUSTOM2
};

struct						s_position
{
	short				col;
	short				row;
};

/*
** CArPOS. Got it? Like Carbon-Argentum-Phosphorus-Oxygen-Sulfur
** Alright, it's __carpos__ for __caret position__
*/
typedef struct s_position	t_carpos;

/*
** s_term stores terminal parameters as well as cursor position and input buffer
*/
struct					s_term
{
	// TODO: Possible memory allocation bottleneck
	char			buffer[MAX_INPUT + 1];
	int				iterator;
	enum e_state	input_state;
	short			ws_row;
	short			ws_col;
	short			tty_fd;
	t_carpos		carpos_db[5];
	struct termios	*original_term;
	struct termios	*current_term;

	short			history_file;
	short			logfile;
};

struct					s_builtin
{
	char	*name;
	int		(*function) (char **);
};

char					**g_environ;
extern struct s_term	*g_term;
extern struct s_builtin	g_builtins[];
pid_t					g_running_process;

//struct command	extract_commands(t_token *tokens);

/*
** Builtins (builtins/ *.c)
*/
int						hs_alias(char **args);
int						hs_cd(char **args);
int						hs_echo(char **args);
int						hs_env(char **args);
int						hs_setenv(char **args);
int						hs_unsetenv(char **args);
int						hs_help(char **args);
int						hs_exit(char **args);
int						hs_where(char **args);
/*
** What is it? A design pattern? Really???
*/
int						execute(char **args);

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
char					**wait_for_input(void);
int						shell_loop(void);
void					setup_signal_handlers(void);
void					display_prompt(enum e_state state);
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
void					free_array(char **array);


#endif
