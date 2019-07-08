/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:50:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/07 16:01:38 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_job_control.h"

static char	**split_to_var(const char *str)
{
	char				tmp[1024];
	int					j;
	char				**splitted;

	ft_bzero(tmp, sizeof(char) * 1024);
	splitted = ft_memalloc(sizeof(char *) * 3);
	j = -1;
	while (str[++j])
		if (str[j] == '=')
		{
			splitted[0] = ft_strsub(str, 0, j);
			splitted[1] = ft_strsub(str, j + 1, ft_strlen(str + j + 1));
			break ;
		}
	return (splitted);
}

static bool	alterate_vars(const struct s_command *command, t_context *context)
{
	const char	**assignments = (const char **)command->assignments;
	char		**splitted;
	int			scope;
	int			i;

	if (command->args == NULL || command->args[0] == NULL)
	{
		scope = g_term->fallback_input_state == STATE_NON_INTERACTIVE ?
			SCOPE_SCRIPT_GLOBAL : SCOPE_SHELL_LOCAL;
		context = g_term->context_original;
	}
	else
		scope = SCOPE_COMMAND_LOCAL;
	i = 0;
	while (assignments && assignments[i])
	{
		splitted = split_to_var(assignments[i]);
		environ_push_entry(context->environ, splitted[0], splitted[1], scope);
		free_array((void **)splitted);
		i++;
	}
	return (assignments && assignments[0]);
}

static void	expand_in_rdrs(const struct s_command *command)
{
	struct s_io_redirect	*rdrs;
	char					*swap;
	int						i;

	rdrs = command->io_redirects;
	i = -1;
	while (rdrs[++i].type != TOKEN_NOT_APPLICABLE)
	{
		if ((swap = rdrs[i].left.path))
			rdrs[i].left.path = expand(rdrs[i].left.path);
		ft_memdel((void **)&swap);
		if ((swap = rdrs[i].right.path))
			rdrs[i].right.path = expand(rdrs[i].right.path);
		ft_memdel((void **)&swap);
	}
}

/*
** TODO: Expand backqoutes and $()
*/

static void	expand_everything(const struct s_command *command)
{
	char					*swap;
	int						i;

	i = -1;
	while (command->args[++i])
	{
		swap = expand(command->args[i]);
		ft_memdel((void **)&(command->args[i]));
		command->args[i] = swap;
	}
	i = -1;
	while (command->assignments[++i])
	{
		swap = command->assignments[i];
		command->assignments[i] = expand(command->assignments[i]);
		ft_memdel((void **)&swap);
	}
	expand_in_rdrs(command);
	expand_globs((struct s_command *)command);
}

/*
** If new_context is not NULL, it must have altered stdin and / or stdout,
** if it was called from pipeline executor. In this case, forknrun routine
** starts processes without waiting them in any way and shell blocks only
** when the last process in the pipeline is passed for execution.
**
** TODO: optimise (how?..) execution if no variables and fds changes are made
*/

int exec_command(const t_node *command_node, t_context *new_context, bool is_async)
{
	const struct s_command	*command = command_node->command;
	t_context				*context;
	int						status;

	if (command_node->node_type != NODE_COMMAND || command == NULL)
		exit(ft_dprintf(2, SH ": fatal: cmd node is not actually a command\n"));
	context = new_context ? new_context
		: context_duplicate(g_term->context_original, XDUP_TERM);
	expand_everything(command);
	alterate_vars(command, context);
	rdr_heredocs(context, command->io_redirects);
	status = 0;
	if (!g_interrupt && command->args != NULL && command->args[0] != NULL)
	{
		jc_tmp_add(process_create((t_command *) command, context));
		is_async = jc_is_subshell() ? false : is_async;
		if (new_context == NULL)
			status = jc_tmp_finalize(is_async);
	}
	else
		context_deep_free(&context);
	return (status);
}
