
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

# define IFNNULL(a, b) if ((a) != NULL) (b);
# define SNWH (copy[i + 1] == '/' || copy[i + 1] == 0 || ft_iswhsp(copy[i + 1]))
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
int						hs_where(char **args);

static char				*g_builtins[] = {
		"alias",
		"cd",
		"echo",
		"env",
		"setenv",
		"unsetenv",
		"help",
		"exit",
		"quit",
		"where"
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
		&hs_exit,
		&hs_where
};

struct s_map			*g_aliases;
char					**g_environ;
/*
** What is it? A design pattern? Really???
*/
int						execute(char **args);
char					*get_env(char *name);
int						set_env(char *key, char *value);
int						unset_env(char *name);
char					**copy_env(char **argenv, char **environ);
void					free_array(char **array);

/*
** Auxilia
*/
void					hs_where_auxilia(char **paths, char *arg);
int						is_valid_var(char *var);
char					*replace_variables(char *line);
char					*replace_home(char *line);

/*
** Memory utils
*/
void	chfree(void *obj);
void	chfree_n(int n, ...);


#endif
