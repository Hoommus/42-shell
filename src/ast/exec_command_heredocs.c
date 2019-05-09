/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_heredocs.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 12:30:39 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/05 18:00:55 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

#define IS_HEREDOC(t) ((t) == TOKEN_DLESS || (t) == TOKEN_DLESSDASH || IS_HW(t))
#define IS_HW(t) ((t) == TOKEN_TRILESS)

static void		concat_heredocs(t_io_rdr *what, t_io_rdr *with_what)
{
	char	*swap;

	swap = ft_strings_join(2, "", what->right.path, with_what->right.path);
	ft_memdel((void **)&(what->right.path));
	ft_memdel((void **)&(with_what->right.path));
	with_what->type = TOKEN_EMPTY;
	what->right.path = swap;
}

void			rdr_heredocs(t_context *context, t_io_rdr *rdrs)
{
	int		pp[2];
	int		fd_search;
	int		i;
	int		j;

	fd_search = -1;
	j = -1;
	while (rdrs[++j].type != TOKEN_NOT_APPLICABLE)
		if (fd_search != rdrs[j].left.fd && (IS_HEREDOC(rdrs->type)))
		{
			i = j;
			fd_search = rdrs[j].left.fd;
			while (rdrs[++i].type != TOKEN_NOT_APPLICABLE)
				if (IS_HEREDOC(rdrs[i].type) && rdrs[i].left.fd == fd_search)
					concat_heredocs(rdrs, (rdrs + i));
		}
	j = -1;
	while (rdrs[++j].type != TOKEN_NOT_APPLICABLE)
		if (IS_HEREDOC(rdrs[j].type))
		{
			pipe(pp);
			context_remove_ofd(context, rdrs[j].left.fd);
			context_add_fd(context, rdrs[j].left.fd, pp[0], "heredoc");
			context_add_fd(context, rdrs[j].right.fd = pp[1], -1, "heredoc");
		}
}
