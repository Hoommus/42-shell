/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 17:42:11 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 17:56:42 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"

int				read_fd(const int fd, char **result)
{
	char		buffer[1025];
	char		*data;
	char		*swap;
	ssize_t		status;

	ft_bzero(buffer, 1025);
	data = ft_strnew(0);
	while ((status = read(fd, buffer, 1024)))
	{
		if (status == -1)
		{
			free(data);
			*result = ft_strnew(0);
			return (-1);
		}
		swap = ft_strjoin(data, buffer);
		free(data);
		data = swap;
		ft_bzero(buffer, 1025);
	}
	*result = data;
	return (1);
}

int				read_filename(const char *file, char **data)
{
	int			fd;
	struct stat	s;

	if (file == NULL)
		return (read_fd(STDIN_FILENO, data) & 0);
	else if (stat(file, &s) != 0)
	{
		ft_dprintf(2, "tokenizer: %s: does not exist.\n", file);
		return (1);
	}
	else if (!S_ISREG(s.st_mode))
	{
		ft_dprintf(2, "tokenizer: %s: ain't a regular file.\n", file);
		return (1);
	}
	fd = open_wrapper(file, O_RDONLY);
	if (fd < 0)
	{
		ft_dprintf(2, "tokenizer: %s: permission denied.\n", file);
		return (1);
	}
	read_fd(fd, data);
	close_wrapper(fd);
	return (0);
}

int				hs_tokenizer(const char **args)
{
	t_token	*tkns;
	t_token	*swap;
	char	*string;

	if (read_filename(args[0], &string))
		return (OK);
	tkns = tokenize(string, TOKEN_DELIMITERS);
	while (tkns)
	{
		if (tkns->type >= TOKEN_WORD && tkns->type <= TOKEN_ASSIGNMENT_WORD)
			ft_printf("(%s %s) ", g_tokens[tkns->type].token_name, tkns->value);
		else
			ft_printf("(%s) ", g_tokens[tkns->type].token_name);
		if (tkns->type == TOKEN_NEWLINE)
			ft_printf("\n");
		swap = tkns;
		tkns = tkns->next;
		free_token(swap);
	}
	ft_printf("\n");
	free(string);
	return (OK);
}
