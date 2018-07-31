#include "../include/minishell.h"

void	free_array(char **array)
{
	int		i;

	i = 0;
	while (array[i])
	{
		chfree(array[i]);
		i++;
	}
	free(array);
}

void	chfree(void *obj)
{
	if (obj != NULL)
		free(obj);
}

void	chfree_n(int n, ...)
{
	va_list		list;
	void		*dummy;

	va_start(list, n);
	while (n)
	{
		if ((dummy = va_arg(list, void *)))
			free(dummy);
		n--;
	}
	va_end(list);
}
