/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxilia.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/25 14:10:25 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"

int			is_valid_var(char *var)
{
	int		i;

	if (var == NULL)
		return (0);
	i = 0;
	while (var[i])
	{
		if (!(ft_isalnum(var[i]) || var[i] == '_' || var[i] == '$'))
			return (0);
		i++;
	}
	return (1);
}

ssize_t		ponies_teleported(void)
{
	ssize_t			ponies;
	static int		fd;

	if (fd == 0)
		fd = open_wrapper("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (1);
	else
	{
		read(fd, &ponies, sizeof(ssize_t));
		if (ponies == 0)
			ponies += 1348;
		return (ABS(ponies));
	}
}

void	display_prompt(enum e_input_state state)
{
	if (state == STATE_NORMAL)
		display_normal_prompt();
	else if (state == STATE_DQUOTE)
		ft_printf("dquote> ");
	else if (state == STATE_QUOTE)
		ft_printf("quote> ");
	else if (state == STATE_ESCAPED_EOL)
		ft_printf("> ");
	else if (state == STATE_HEREDOC)
		ft_printf("heredoc> ");
	else if (state == STATE_EMPTY_PIPE)
		ft_printf("pipe> ");
	else
		ft_printf("err> ");
	carpos_update(POS_PROMPT);
}

bool	is_string_numeric(const char *str, const int base)
{
	static char		*numbers = "0123456789ABCDEF";
	bool			is_found;
	int				i;

	is_found = false;
	while (*str)
	{
		i = 0;
		while (i < base && !is_found)
			if (*str == numbers[i++])
				is_found = true;
		if (!is_found)
			return (false);
		str++;
	}
	return (true);
}

