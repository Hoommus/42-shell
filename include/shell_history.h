#ifndef SHELL_HISTORY_H
# define SHELL_HISTORY_H

# include <stdlib.h>
# include <time.h>
# include "libft.h"
# include "get_next_line.h"
# include "ft_printf.h"

# define INITIAL_HISTORY_SIZE 256

struct						s_history_entry
{
	char		*command;
	time_t		timestamp;
};

typedef struct				s_history_vector
{
	struct s_history_entry	**entries;
	u_int64_t				size;
	u_int64_t				capacity;
	u_int64_t				iterator;
}							t_history;

/*
** History utilities (features/history.c, features/history_vector.c)
*/

void						init_history_vector(const u_int64_t capacity);
void						push_history_entry(const char *cmd, time_t stamp);
void						save_history_entry(int fd);
void						load_history(int fd);
struct s_history_entry		*pop_history_entry(void);
struct s_history_entry		*get_history_entry(u_int64_t index);
char						*write_history(char *command, int history_file);

void						chfree_n(int n, ...);

#endif
