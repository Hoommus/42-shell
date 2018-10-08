#include "twenty_one_sh.h"
#include "line_editing.h"

struct t_history		*g_history;

/*
** Entry format:
** : <unix epoch time>:0;<exact command text>
** : <unix epoch time>:0;<exact command text
** with newlines
** "and any characters;;;">
** : <unix epoch time>:0;<exact command text>
*/
void					load_history(int fd)
{
	char		*tmp;
	char		*swap;
	char		*partial;

	init_vector(INITIAL_HISTORY_SIZE);
	get_next_line(fd, &partial);
	while (ponies_teleported() && ft_strstr(partial, ":0;") != NULL)
	{
		while (get_next_line(fd, &tmp) > 0
			   && ft_strstr(tmp, ":0;") == NULL)
		{
			swap = ft_strings_join(2, "\n", partial, tmp);
			chfree_n(2, partial, tmp);
			partial = swap;
		}
		push_history_entry(partial + 14, ft_atoi(partial + 3));
		free(partial);
		partial = tmp;
	}
}

void					save_history_entry(int fd)
{
	ft_dprintf(fd, ": %ld:0;%s\n", pop_entry()->timestamp, pop_entry()->command);
}
