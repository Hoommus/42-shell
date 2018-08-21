#include "../../include/twenty_one_sh.h"
#include "../../include/line_editing.h"

int				is_printable(long c)
{
	if ((c >= 9 && c <= 13) || (c >= 32 && c != 127 && c < 0x10FFFF))
		return (1);
	else
		return (0);
}

char			*get_buffer(void)
{
	return (ft_strdup((char *)g_term->line_buffer));
}

void			clear_buffer(void)
{
	g_term->iterator = 0;
	ft_bzero(g_term->line_buffer, (1 + MAX_INPUT) * sizeof(unsigned char));
}

int				bufferize_char(char chr)
{
	if (g_term->iterator < MAX_INPUT && chr)
		g_term->line_buffer[g_term->iterator++] = (unsigned char)chr;
	else if (g_term->iterator == MAX_INPUT)
		return (ERR);
	return (OK);
}

int				bufferize_string(char *str)
{
	while (g_term->iterator < MAX_INPUT && str && *str)
		g_term->line_buffer[g_term->iterator++] = (unsigned char)(*str++);
	if (g_term->iterator == MAX_INPUT)
		return (ERR);
	return (OK);
}

//struct command	*extract_commands(char *str)
//{
//	struct command		*commands;
//	char				**splitted;
//
//}

char			**wait_for_input(void)
{
	long	chr;
	char	*str;
	char	**commands;

	commands = NULL;
	while (!commands && g_term->state != BREAK)
	{
		chr = 0;
		read(0, &chr, sizeof(long));
		if (chr == '\n' && g_term->state == NORMAL)
		{
			str = get_buffer();
			commands = smart_split(str, TOKEN_DELIMITERS);
			free(str);
			//ft_printf("\n");
		}
		else if (is_printable(chr) || g_term->state != NORMAL)
		{
			str = to_unicode(chr);
			bufferize_string(str);
			free(str);
		}
		else if (chr == CINTR)
			kill(0, SIGINT);
		else
			handle_key(chr);
		write(3, &chr, sizeof(long));
	}
	return (commands);
}
