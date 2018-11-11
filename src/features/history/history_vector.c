#include "shell_history.h"

extern t_history		*g_history;

void					init_history_vector(const u_int64_t capacity)
{
	g_history = (t_history *)ft_memalloc(sizeof(t_history));
	g_history->capacity = capacity;
	g_history->size = 0;
	g_history->iterator = 0;
	g_history->entries = (struct s_history_entry **)
			ft_memalloc(capacity * sizeof(struct s_history_entry *));
}

static void				reallocate_history_vector(void)
{
	struct s_history_entry	**tmp;
	u_int64_t				sz;

	g_history->capacity += g_history->capacity / 2;
	tmp = (struct s_history_entry **)
			ft_memalloc(sizeof(struct s_history_entry *) * g_history->capacity);
	sz = sizeof(struct s_history_entry *) * g_history->size;
	ft_memcpy(tmp, g_history->entries, sz);
	free(g_history->entries);
	g_history->entries = tmp;
}

void					push_history_entry(const char *cmd, time_t timestamp)
{
	struct s_history_entry	*entry;

	if (g_history->size >= g_history->capacity - 1)
		reallocate_history_vector();
	entry = (struct s_history_entry *)
			ft_memalloc(sizeof(struct s_history_entry));
	entry->command = ft_strdup(cmd);
	entry->timestamp = timestamp;
	g_history->entries[(g_history->size)++] = entry;
}

struct s_history_entry	*get_history_entry(u_int64_t index)
{
	if (index >= g_history->size)
		return (NULL);
	return (g_history->entries[index]);
}

struct s_history_entry	*pop_history_entry(void)
{
	return (g_history->entries[g_history->size - 1]);
}
