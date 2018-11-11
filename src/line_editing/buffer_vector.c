#include "buffer_works.h"
#include "twenty_one_sh.h"

t_buffer		*g_buffer;

void			init_buffer_vector(const u_int64_t capacity)
{
	g_buffer = (t_buffer *)ft_memalloc(sizeof(t_buffer));
	g_buffer->array = (t_symbol *)ft_memalloc(sizeof(t_symbol)
			* (capacity + 1));
	g_buffer->capacity = capacity;
	g_buffer->iterator = 0;
	g_term->v_buffer = g_buffer;
}

void			relocate_buffer_vector(void)
{
	t_symbol	*tmp;
	u_int64_t	sz;

	g_buffer->capacity += g_buffer->capacity / 2;
	tmp = (t_symbol *)ft_memalloc(sizeof(t_symbol) * g_buffer->capacity);
	sz = sizeof(t_symbol) * g_buffer->size;
	ft_memcpy(tmp, g_buffer->array, sz);
	free(g_buffer->array);
	g_buffer->array = tmp;
}

int				insert_symbol_at(u_int64_t index, t_symbol *symbol)
{
	if (g_buffer->size >= g_buffer->capacity - 2)
		relocate_buffer_vector();
	if (index == g_buffer->size)
		g_buffer->array[g_buffer->iterator] = *symbol;
	else
	{
		ft_memmove(g_buffer->array + index + 1, g_buffer->array + index,
				   (g_buffer->size - index) * sizeof(t_symbol));
		ft_memcpy(g_buffer->array + index, symbol, sizeof(t_symbol));
	}
	g_buffer->size += 1;
	g_buffer->iterator += 1;
	return (OK);
}

int				insert_string_at(u_int64_t index, const char str[8])
{
	t_symbol	symbol;

	ft_memcpy(symbol.s, str, 8);
	return (insert_symbol_at(index, &symbol));
}