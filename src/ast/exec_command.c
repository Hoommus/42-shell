/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:50:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/10 19:35:41 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"
#include "shell_script_parser.h"
#include "shell_job_control.h"

#include <assert.h>
char	**split_to_var(const char *str)
{
	t_token		*swap;
	t_token		*tokens;
	char		**splitted;

	tokens = tokenize((char *)str, "=");
	splitted = ft_memalloc(sizeof(char *) * 3);
	splitted[0] = (char *)(tokens->value);
	if (tokens->next)
		splitted[1] = (char *)(tokens->next->value);
	else
		splitted[1] = NULL;
	while (tokens)
	{
		swap = tokens->next;
		free(tokens);
		tokens = swap;
	}
	return (splitted);
}

bool	alterate_vars(const struct s_command *command, t_context *context)
{
	const char	**assignments = (const char **)command->assignments;
	char		**splitted;
	int			scope;
	int			i;

	if (command->args == NULL || command->args[0] == NULL)
		scope = SCOPE_SHELL_LOCAL;
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

/*
** TODO: Expand backqoutes and $()
*/
void	expand_everything(const struct s_command *command)
{
	char	*swap;
	int		i;

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
//	i = 0;
//	while (command->io_redirects + i)
//	{
// 		swap = ((command->io_redirects) + i)->what.path;
// 		if (swap)
//			((command->io_redirects) + i)->what.path =
//				expand(((command->io_redirects) + i)->what.path);
//		ft_memdel((void **)&swap);
// 		swap = ((command->io_redirects) + i)->where.path;
// 		if (swap)
//			((command->io_redirects) + i)->where.path =
//				expand(((command->io_redirects) + i)->where.path);
//		ft_memdel((void **)&swap);
//		i++;
//	}
}

/*
** If new_context is not NULL, it must have altered stdin and / or stdout,
** if it was called from pipeline executor. In this case, forknrun routine
** starts processes without waiting them in any way and shell blocks only
** when the last process in the pipeline is passed for execution.
**
** The incapsulation of command execution is fucked while inside pipe, because
** Bash script is stupid as fuck and I wish there was better script language.
** See alterate_filedes() function, which also must be called before pipe exec.
*/

// TODO: optimise execution if no variables and fds changes are made
int		exec_command(const t_node *command_node, t_context *new_context)
{
	const struct s_command	*command = command_node->command;
	t_context				*context;
	int						status;

	context = new_context ? new_context
							: context_duplicate(g_term->context_original, true);
	// TODO: Expand environment variables in args, assignments et cetera
	expand_everything(command);
	ft_printf("+{");
	for (int i = 0; command->assignments[i] != NULL; i++)
		ft_printf("%s ", command->assignments[i]);
	for (int i = 0; command->args[i] != NULL; i++)
		ft_printf((command->args[i + 1] != NULL) ? "%s " : "%s", command->args[i]);
	ft_printf("}\n");
	if (!new_context)
		alterate_filedes(command, context);
	alterate_vars(command, context);
	if (!g_is_interrupted)
	{
		jc_enqueue_job(jc_create_job(command->args, context, (bool)new_context));
		status = !new_context ? jc_execute_queue() : 0;
		if (!new_context)
			jc_destroy_queue();
	}
	else
		status = 0;
	// context should be switched back and freed on job destruction
	// context_deep_free(&context);
	// context_switch(NULL);
	return (status);
}
