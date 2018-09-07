#include "../../include/twenty_one_sh.h"
#include "../../include/line_editing.h"
#include "../../include/script_lang.h"

int				is_printable(const char c[8])
{
	if ((c[0] >= 10 && c[0] <= 13) || (c[0] >= 32 && c[0] != 127))
		return (1);
	else if (((c[0] & 0xC0) && (c[1] & 0x80))
		|| ((c[0] & 0xE0) && (c[1] & 0x80) && (c[2] & 0x80))
		|| ((c[0] & 0xF0) && (c[1] & 0x80) && (c[2] & 0x80) && (c[3] & 0x80)))
		return (1);
	else
		return (0);
}

void			clear_buffer(char symbol)
{
	if (symbol == 0)
	{
		g_term->iterator = 0;
		ft_bzero(g_term->buffer, (1 + MAX_INPUT) * sizeof(unsigned char));
	}
	else
	{
		ft_bzero(g_term->buffer + g_term->iterator -
						ft_strchr_back(g_term->buffer, '\n', g_term->iterator),
				MAX_INPUT - g_term->iterator);
	}
	g_term->input_state = NORMAL;
}

int				delete_char_at(char *str, int64_t index)
{
	int		size;

	size = 1;
	while (((uint8_t)str[index]) > 0x7F
		   && (str[index] & 0xC0) == (str[index] & 0x80))
	{
		index--;
		size++;
	}
	ft_memmove(str + index, str + index + size, ft_strlen(str + index));
	return (size);
}

int				insert_string_at(char *str, const char *substr, int64_t index)
{
	size_t	len;

	len = ft_strlen((char *)substr);
	ft_memmove(str + index + len, str + index, ft_strlen(str + index));
	ft_memcpy(str + index, substr, len);
	return (OK);
}

/*
** Returns number of 0b10xxxxxx-based characters
*/
uint64_t		get_utf_body_size(char first)
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

void			deal_with_printable(const char arr[8])
{
	if (arr[0] == '\'')
		toggle_quote();
	else if (arr[0] == '"')
		toggle_dquote();
	else if (arr[0] == '\\')
		toggle_escaped();
	else if (arr[0] == '`')
		toggle_bquote();
	else if (g_term->input_state == ESCAPED_NL)
		g_term->input_state = NORMAL;
	insert_string_at(g_term->buffer, arr, g_term->iterator);
	g_term->iterator += ft_strlen((char *)arr);
	tputs(tgetstr("im", NULL), 1, &ft_putc);
	write(1, arr, 8);
	tputs(tgetstr("ei", NULL), 1, &ft_putc);
	redraw_buffer();
}

void			deal_with_newline(const char arr[8])
{
	if (g_term->input_state == QUOTE
		|| g_term->input_state == DQUOTE
		|| g_term->input_state == ESCAPED_NL)
	{
		write(STDOUT_FILENO, "\n", 1);
		display_prompt(g_term->input_state);
		insert_string_at(g_term->buffer, (char *)arr, g_term->iterator++);
		redraw_buffer();
	}
	else if (g_term->buffer[g_term->iterator] == '\0')
	{
		write(STDOUT_FILENO, "\n", 1);
		g_term->input_state = COMMIT;
	}
	else
	{
		caret_move(ft_strlen(g_term->buffer + g_term->iterator), D_RIGHT);
		write(STDOUT_FILENO, "\n", 1);
		g_term->input_state = COMMIT;
	}
}

/*
** TODO: Make this one handle long input from Command + V
*/
char			**wait_for_input(void)
{
	char		**commands;
	long		chr;
	uint64_t	status;
	char		arr[8];

	commands = NULL;
	while (!commands)
	{
		ft_bzero(arr, 8 * sizeof(char));
		read(0, arr, 1);
		status = get_utf_body_size(arr[0]);
		if (status != 0)
			read(0, arr + 1, status);
		ft_memcpy(&chr, arr,  8);
		if (arr[0] == '\n')
			deal_with_newline(arr);
		else if (is_printable(arr) && arr[0] != '\n')
			deal_with_printable(arr);
		else
			handle_key(chr);
		if (g_term->input_state == COMMIT)
			commands = smart_split(g_term->buffer, TOKEN_DELIMITERS);
		update_caret_position(POS_CURRENT);
	}
	return (commands);
}
