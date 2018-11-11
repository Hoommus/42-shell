#include "buffer_works.h"

extern t_buffer	*g_buffer;

int				buff_strchr_back(t_buffer *buffer, const char *c, int i)
{
	while (i >= 0)
	{
		if (ft_strcmp(buffer->array[i].s, c) == 0)
			break;
		i--;
	}
	return (i);
}

t_symbol		*buff_symbol_at(const u_int64_t index)
{
	return (g_buffer->array + index);
}

char			*buff_char_at(const u_int64_t index)
{
	return (g_buffer->array[index].s);
}

int				buff_char_at_equals(const u_int64_t index, const char *c)
{
	if (c == NULL || ft_strlen((char *)c) > 8)
		return (0);
	return (ft_strcmp(g_buffer->array[index].s, c));
}

