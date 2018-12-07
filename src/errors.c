#include "twenty_one_sh.h"

void	throw_fatal(char *cause)
{
	int size;

	size = (int)(ft_strlen(cause) > (uint)g_term->ws_col ? ft_strlen(cause) + 1
													: g_term->ws_col);
	ft_printf("\x1b[41;1m%-*s\x1b[0;0m\n", size, cause);
}
