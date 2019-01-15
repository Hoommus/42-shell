#include "../../include/twenty_one_sh.h"
#include "line_editing.h"

int		toggle_quote(void)
{
	if (g_term->input_state == STATE_QUOTE)
		return (g_term->input_state = STATE_NORMAL);
	else if (g_term->input_state == STATE_NORMAL)
		return (g_term->input_state = STATE_QUOTE);
	return (0);
}

int		toggle_dquote(void)
{
	if (g_term->input_state == STATE_DQUOTE)
		return (g_term->input_state = STATE_NORMAL);
	else if (g_term->input_state == STATE_NORMAL)
		return (g_term->input_state = STATE_DQUOTE);
	return (0);
}

int		toggle_bquote(void)
{
	if (g_term->input_state == STATE_BQUOTE)
		return (g_term->input_state = STATE_NORMAL);
	else if (g_term->input_state == STATE_NORMAL)
		return (g_term->input_state = STATE_BQUOTE);
	return (0);
}

int		toggle_escaped(void)
{
	if (g_term->input_state == STATE_ESCAPED_EOL)
		return (g_term->input_state = STATE_NORMAL);
	else if (g_term->input_state == STATE_NORMAL)
		return (g_term->input_state = STATE_ESCAPED_EOL);
	return (0);
}

int		toggle_state(const char *c)
{
	if (ft_strcmp(c, "\'") == 0)
		return (toggle_quote());
	else if (ft_strcmp(c, "\"") == 0)
		return (toggle_dquote());
	else if (ft_strcmp(c, "`") == 0)
		return (toggle_bquote());
	else if (ft_strcmp(c, "\\") == 0 &&
			g_term->buffer->iterator == g_term->buffer->size)
		return (toggle_escaped());
	else
		return (0);
}
