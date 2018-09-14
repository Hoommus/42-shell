#include "../../include/twenty_one_sh.h"
#include "../../include/line_editing.h"

void	toggle_quote(void)
{
	if (g_term->input_state == STATE_QUOTE)
		g_term->input_state = STATE_NORMAL;
	else if (g_term->input_state == STATE_NORMAL)
		g_term->input_state = STATE_QUOTE;
}

void	toggle_dquote(void)
{
	if (g_term->input_state == STATE_DQUOTE)
		g_term->input_state = STATE_NORMAL;
	else if (g_term->input_state == STATE_NORMAL)
		g_term->input_state = STATE_DQUOTE;
}

void	toggle_bquote(void)
{
	if (g_term->input_state == STATE_BQUOTE)
		g_term->input_state = STATE_NORMAL;
	else if (g_term->input_state == STATE_NORMAL)
		g_term->input_state = STATE_BQUOTE;
}

void	toggle_escaped(void)
{
	if (g_term->input_state == STATE_ESCAPED_NL)
		g_term->input_state = STATE_NORMAL;
	else if (g_term->input_state == STATE_NORMAL)
		g_term->input_state = STATE_ESCAPED_NL;
}
