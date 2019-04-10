/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 17:42:11 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/16 17:42:11 by vtarasiu         ###   ########.fr       */
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
			return (0);
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
	{
		read_fd(STDIN_FILENO, data);
		return (0);
	}
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
		ft_dprintf(2, "tokenizer: %s: could not open a file for reading.\n", file);
		return (1);
	}
	read_fd(fd, data);
	return (0);
}

int				hs_tokenizer(const char **args)
{
	t_token	*tokens;
	t_token	*head;
	t_token	*swap;
	char	*string;

	TERM_APPLY_CONFIG(g_term->context_original->term_config);
	if (read_filename(args[0], &string))
	{
		TERM_APPLY_CONFIG(g_term->context_current->term_config);
		return (OK);
	}
	head = tokenize(string, TOKEN_DELIMITERS);
	tokens = head;
	while (tokens)
	{
		if (tokens->type >= TOKEN_WORD && tokens->type <= TOKEN_ASSIGNMENT_WORD)
			ft_printf("(%s %s) ", g_tokens[tokens->type].token_name, tokens->value);
		else
			ft_printf("(%s) ", g_tokens[tokens->type].token_name);
		if (tokens->type == TOKEN_NEWLINE)
			ft_printf("\n");
		swap = tokens;
		tokens = tokens->next;
		free_token(swap);
	}
	ft_printf("\n");
	free(string);
	TERM_APPLY_CONFIG(g_term->context_current->term_config);
	return (OK);
}
