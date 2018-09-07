#include "../../include/twenty_one_sh.h"
#include "../../include/line_editing.h"

void	toggle_quote(void)
{
	if (g_term->input_state == QUOTE)
		g_term->input_state = NORMAL;
	else if (g_term->input_state == NORMAL)
		g_term->input_state = QUOTE;
}

void	toggle_dquote(void)
{
	if (g_term->input_state == DQUOTE)
		g_term->input_state = NORMAL;
	else if (g_term->input_state == NORMAL)
		g_term->input_state = DQUOTE;
}

void	toggle_bquote(void)
{
	if (g_term->input_state == BQUOTE)
		g_term->input_state = NORMAL;
	else if (g_term->input_state == NORMAL)
		g_term->input_state = BQUOTE;
}

void	toggle_escaped(void)
{
	if (g_term->input_state == ESCAPED_NL)
		g_term->input_state = NORMAL;
	else if (g_term->input_state == NORMAL)
		g_term->input_state = ESCAPED_NL;
}
