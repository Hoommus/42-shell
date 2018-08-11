#include "../include/twenty_one_sh.h"

int		is_printable(wchar_t chr)
{
	if (chr > 0x10FFFF)
		return (0);
	else if (chr >= 32 && chr != 127)
		return (1);
	return (0);
}

char	*bufferize_line(char *s, long long len)
{
	static char		buffer[MAX_INPUT];
	static long		i;
	char			*final;

	while (i < MAX_INPUT && len--)
	{
		buffer[i++] = (unsigned char)*s;
		s++;
	}
	if (s == NULL && len == -1)
	{
		final = ft_strdup(&(*buffer));
		i = 0;
		ft_bzero(buffer, MAX_INPUT * sizeof(unsigned char));
		return (final);
	}
	else if (i == MAX_INPUT)
	{
		ft_bzero(buffer, MAX_INPUT);
		i = 0;
	}
	return (NULL);
}

char	**wait_for_input(void)
{
	wchar_t			chr;
	char			*str;
	char			**commands;

	commands = NULL;
	while (ponies_teleported())
	{
		chr = 0;
		read(0, &chr, sizeof(long long));
		if (chr == '\n')
		{
			str = bufferize_line(0, -1);
			commands = ft_strsplit(str, ' ');
			free(str);
			return (commands);
		}
		else if (is_printable(chr))
		{
			str = to_unicode(chr);
			bufferize_line(str, ft_strlen(str));
			free(str);
		}
		else
			handle_key(chr);
	}
	return (commands);
}
