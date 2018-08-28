#include "../../include/twenty_one_sh.h"
#include "../../include/line_editing.h"

int				is_printable(const char c[8])
{
	if ((c[0] >= 9 && c[0] <= 13) || (c[0] >= 32 && c[0] != 127))
		return (1);
	else if (((c[0] & 0xC0) && (c[1] & 0x80))
		|| ((c[0] & 0xE0) && (c[1] & 0x80) && (c[2] & 0x80))
		|| ((c[0] & 0xE0) && (c[1] & 0x80) && (c[2] & 0x80) && (c[3] & 0x80)))
		return (1);
	else
		return (0);
}

void			clear_buffer(void)
{
	g_term->iterator = 0;
	ft_bzero(g_term->buffer, (1 + MAX_INPUT) * sizeof(unsigned char));
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
	//size = size == 1 ? 1 : size;
	ft_memmove(str + index, str + index + size, ft_strlen(str + index));
	return (size);
}

int				insert_char_at(char *str, char c, int64_t index)
{
	if (index < (int64_t)ft_strlen(str))
		ft_memmove(str + index + 1, str + index, ft_strlen(str + index));
	*(str + index) = c;
	return (OK);
}

int				insert_string_at(char *str, char *substr, int64_t index)
{
	size_t	len;

	len = ft_strlen(substr);
	ft_memmove(str + index + len, str + index, ft_strlen(str + index));
	ft_memcpy(str + index, substr, len);
	return (OK);
}

void			deal_with_printable(const char arr[8])
{
	if (g_term->state != NORMAL)
		display_prompt(g_term->state);
	insert_string_at(g_term->buffer, (char *)arr, g_term->iterator);
	g_term->iterator += ft_strlen((char *)arr);
	tputs(tgetstr("im", NULL), 1, &ft_putc);
	write(1, arr, 8);
	tputs(tgetstr("ei", NULL), 1, &ft_putc);
}

char			**wait_for_input(void)
{
	long	chr;
	char	arr[8];
	char	**commands;

	commands = NULL;
	while (!commands)
	{
		ft_bzero(arr, 8);
		ft_memcpy(&chr, arr, read(0, arr, 8) == -1 ? 0 : 8);
		if (arr[0] == '\n' && g_term->state == NORMAL)
		{
			commands = smart_split((char *)g_term->buffer, TOKEN_DELIMITERS);
			write(STDOUT_FILENO, "\n", 1);
		}
		else if (is_printable(arr) || g_term->state != NORMAL)
			deal_with_printable(arr);
		else
			handle_key(chr);
		update_cursor_position();
	}
	return (commands);
}
