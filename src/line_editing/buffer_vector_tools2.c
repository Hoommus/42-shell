#include "buffer_works.h"

extern t_buffer	*g_buffer;

/*
** Returns last desired char's offset in buffer searching from index 'i' to 0
** If nothing found, returns -1
*/

int				buff_chroff(t_buffer *buffer, const char *c, int i)
{
	while (i >= 0)
	{
		if (ft_strcmp(buffer->array[i].s, c) == 0)
			break ;
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
	return (!ft_strcmp(g_buffer->array[index].s, c));
}


/*
** Returns number of 0b10xxxxxx-based characters
*/
u_int64_t		utf_body_size(char first)
{
	unsigned char	c;

	c = (unsigned char)first;
	if (c >= 32 && c < 127)
		return (0);
	if ((c & 0xC0) == 0xC0)
		return (1);
	else if ((c & 0xE0) == 0xE0)
		return (2);
	else if ((c & 0xF0) == 0xF0)
		return (3);
	else if (c == 27)
		return (7);
	else
		return (0);
}
