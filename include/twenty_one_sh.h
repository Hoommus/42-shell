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
# include "script_lang.h"
#include "line_editing.h"

# define SNWH (copy[i + 1] == '/' || copy[i + 1] == 0 || ft_iswhsp(copy[i + 1]))
# define ABS(a) ((a) < 0 ? -(a) : (a))
# define PROMPT "\x001b[0m\x001b[38;5;2m[%s@%s] \x001b[37;1m%s $> \x001b[0m"

/*
** ◦ pipe
** ◦ dup, dup2
** ◦ isatty, ttyname, ttyslot
** ◦ ioctl
** ◦ getenv
** ◦ tcsetattr, tcgetattr
** ◦ tgetent
** ◦ tgetflag
** ◦ tgetnum
** ◦ tgetstr
** ◦ tgoto
** ◦ tputs
*/

// TODO: Remove after AST implemented
struct			command {
	char	*command;
	char	**args;
};

/*
** Real... e_state
** I mean, used for controlling of input state
*/

enum				e_state
{
	NORMAL,
	QUOTE,
	DQUOTE,
	HEREDOC,
	ESCAPED_NL,
	NEXT_ESCAPED,
	BREAK
};

/*
** s_term stores terminal parameters as well as cursor position and input buffer
*/
struct					s_term
{
	// TODO: Possible memory allocation bottleneck
	char			line_buffer[MAX_INPUT + 1];
	uint64_t		iterator;
	enum e_state	state;
	short			term_rows;
	short			term_cols;
	short			cursor_row;
	short			cursor_col;
};

typedef int				(*t_builtin_func) (char **);

extern struct s_term	*g_term;
extern char				*g_builtins[];
extern t_builtin_func	g_builtin_func[];

pid_t					g_running_process;
char					**g_environ;

//struct command	extract_commands(t_token *tokens);


/*
** Builtins
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
** Main Loop
*/
char					**wait_for_input(void);
int						shell_loop(void);
void					setup_signal_handlers(void);

/*
** Auxilia
*/
ssize_t					ponies_teleported(void);
void					increment_shlvl(void);

/*
** Final input parsing
*/
char					*replace_variables(char *line);
char					*replace_home(char *line);
void					restore_variables(void);
void					expand_variables(char **line);
int						is_valid_var(char *var);

/*
** Memory utils
*/
void					chfree(void *obj);
void					chfree_n(int n, ...);
void					free_array(char **array);


#endif
