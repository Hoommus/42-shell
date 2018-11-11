#include "shell_history.h"

t_history	*g_history;

/*
** Entry format:
** : <unix epoch time>:0;<exact command text>
** : <unix epoch time>:0;<exact command text
** with newlines
** "and any characters;;;">
** : <unix epoch time>:0;<exact command text>
*/
void		load_history(int fd)
{
	char		*tmp;
	char		*swap;
	char		*partial;

	init_history_vector(INITIAL_HISTORY_SIZE);
	if (get_next_line(fd, &partial) <= 0)
		return ;
	while (ft_strlen(partial) > 0 && ft_strstr(partial, ":0;") != NULL)
	{
		while (get_next_line(fd, &tmp) > 0 && ft_strstr(tmp, ":0;") == NULL)
		{
			swap = ft_strings_join(2, "\n", partial, tmp);
			chfree_n(2, partial, tmp);
			partial = swap;
		}
		push_history_entry(partial + 14, ft_atoi(partial + 3));
		chfree_n(1, partial);
		partial = tmp;
		tmp = NULL;
	}
}

void		save_history_entry(int fd)
{
	ft_dprintf(fd, ": %10.0ld:0;%s\n", pop_history_entry()->timestamp,
										pop_history_entry()->command);
}

char		*write_history(char *command, int history_file)
{
	time_t		timestamp;

	time(&timestamp);
	push_history_entry(command, timestamp);
	save_history_entry(history_file);
	return (command);
}
