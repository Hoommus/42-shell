
#ifndef MINISHELL_H
# define MINISHELL_H

#include <string.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
#include <sys/ioctl.h>

# include "../printf/include/ft_printf.h"
# include "../libft/libft.h"
# include "../libft/get_next_line.h"

# define ABS(a) (a < 0 ? -(a) : a)
# define PROMPT "\x001b[40\x001b[38;5;2m[%s@%s] \x001b[37;1m%s $> \x001b[0m"
/*
** access
** open, close, read, write
** opendir, readdir, closedir
** getcwd, chdir
** stat, lstat, fstat
** fork, execve
** wait, waitpid, wait3, wait4
** signal, kill
*/

typedef int				(*t_builtin_func) (char **);

typedef struct			s_map
{
	char			*key;
	char			*value;
	struct s_map	*next;
}						t_map;

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

static char				*g_builtins[] = {
		"alias",
		"cd",
		"echo",
		"env",
		"setenv",
		"unsetenv",
		"help",
		"exit"
};

static t_builtin_func	g_builtin_func[] = {
		&hs_alias,
		&hs_cd,
		&hs_echo,
		&hs_env,
		&hs_setenv,
		&hs_unsetenv,
		&hs_help,
		&hs_exit,
};

struct s_map			*g_aliases;

/*
** What is it? A design pattern?? Really?
*/
int						execute(char **args, char **paths);
char					*get_env(char *name, char **env);





#endif
