#include "../../include/twenty_one_sh.h"
#include "../../include/line_editing.h"

extern t_history		*g_history;

/*
** Fast memcopy - 8 bytes at a time;
*/
void					*ft_fmemcpy(void *dst, void *src, uint64_t size)
{
	uint64_t	*d;
	uint64_t	*s;
	size_t		i;

	i = 0;
	d = (uint64_t *)dst;
	s = (uint64_t *)src;
	while (i < size)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

void					init_vector(const uint64_t capacity)
{
	g_history = (t_history *)ft_memalloc(sizeof(t_history));
	g_history->capacity = capacity;
	g_history->entries = (struct s_history_entry **)
			ft_memalloc(capacity * sizeof(struct s_history_entry *));
}

void					relocate_vector(void)
{
	struct s_history_entry	**tmp;
	uint64_t				sz;

	g_history->capacity += g_history->capacity / 2;
	tmp = (struct s_history_entry **)
			ft_memalloc(sizeof(struct s_history_entry *) * g_history->capacity);
	sz = sizeof(struct s_history_entry *) * g_history->size;
	if (sz % 8 == 0)
		ft_fmemcpy(tmp, g_history->entries, sz);
	else
		ft_memcpy(tmp, g_history->entries, sz);
	free(g_history->entries);
	g_history->entries = tmp;
}

void					push_history_entry(const char *cmd, time_t timestamp)
{
	struct s_history_entry	*entry;

	if (g_history->size >= g_history->capacity - 1)
		relocate_vector();
	entry = (struct s_history_entry *)
			ft_memalloc(sizeof(struct s_history_entry));
	entry->command = ft_strdup(cmd);
	entry->timestamp = timestamp;
	g_history->entries[++g_history->size] = entry;
	save_history_entry(g_term->history_file);
}

struct s_history_entry	*get_entry(uint64_t index)
{
	return (g_history->entries[index]);
}

struct s_history_entry	*pop_entry(void)
{
	return (g_history->entries[g_history->size - 1]);
}
