/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxilia.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/19 13:22:23 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"

bool		is_valid_var(const char *var)
{
	int		i;

	if (var == NULL || ft_isdigit(*var) || ft_strlen(var) == 0)
		return (false);
	i = 0;
	while (var[i])
	{
		if (!(ft_isalnum(var[i]) || var[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

ssize_t		ponies_teleported(void)
{
	ssize_t			ponies;
	int				fd;

	fd = open_wrapper("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (1);
	else
	{
		read(fd, &ponies, sizeof(ssize_t));
		if (ponies == 0)
			ponies += 1348;
		close(fd);
		return (ABS(ponies));
	}
}

void		display_prompt(enum e_input_state state)
{
	if (carpos_update(POS_CURRENT)->col > 1)
		ft_printf("\n");
	if (state == STATE_NORMAL)
		display_normal_prompt();
	else if (state == STATE_DQUOTE)
		ft_printf("dquote> ");
	else if (state == STATE_QUOTE)
		ft_printf("quote> ");
	else if (state == STATE_ESCAPED)
		ft_printf("> ");
	else if (state == STATE_HEREDOC)
		ft_printf("hdoc %s> ", g_term->heredoc_word);
	else if (state == STATE_HEREDOCD)
		ft_printf("hdocd %s> ", g_term->heredoc_word);
	else if (state == STATE_EMPTY_PIPE)
		ft_printf("pipe> ");
	else
		ft_printf("err> ");
	carpos_update(POS_PROMPT);
}

bool		is_string_numeric(const char *str, const int base)
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

bool		is_dir(const char *path)
{
	struct stat		s;

	if (stat(path, &s) == -1)
		return (false);
	return (S_ISDIR(s.st_mode));
}
