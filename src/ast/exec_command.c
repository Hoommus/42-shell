/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:50:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/26 15:56:35 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_job_control.h"

static char	**split_to_var(const char *str)
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

static bool	alterate_vars(const struct s_command *command, t_context *context)
{
	const char	**assignments = (const char **)command->assignments;
	char		**splitted;
	int			scope;
	int			i;

	if (command->args == NULL || command->args[0] == NULL)
	{
		scope = SCOPE_SHELL_LOCAL;
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

/*
** TODO: Expand backqoutes and $()
*/

static void	expand_everything(const struct s_command *command)
{
	struct s_io_redirect	*rdrs;
	char					*swap;
	int						i;

	rdrs = command->io_redirects;
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
	i = -1;
	while (rdrs[++i].type != TOKEN_NOT_APPLICABLE)
	{
 		if ((swap = rdrs[i].what.path))
			rdrs[i].what.path = expand(rdrs[i].what.path);
		ft_memdel((void **)&swap);
 		if ((swap = rdrs[i].where.path))
			rdrs[i].where.path = expand(rdrs[i].where.path);
		ft_memdel((void **)&swap);
	}
}

/*
** If new_context is not NULL, it must have altered stdin and / or stdout,
** if it was called from pipeline executor. In this case, forknrun routine
** starts processes without waiting them in any way and shell blocks only
** when the last process in the pipeline is passed for execution.
**
** TODO: optimise (how?..) execution if no variables and fds changes are made
*/

int		exec_command(const t_node *command_node, t_context *new_context)
{
	const struct s_command	*command = command_node->command;
	t_context				*context;
	enum e_job_state		job_class;
	int						status;

	job_class = JOB_FG;
	if (new_context)
	{
		context = new_context;
		job_class = command->is_async ? JOB_BG : JOB_PIPE;
	}
	else
		context = context_duplicate(g_term->context_original, true);
	expand_everything(command);
	ft_printf("+{ ");
	for (int i = 0; command->assignments[i] != NULL; i++)
		ft_printf("%s ", command->assignments[i]);
	for (int i = 0; command->args[i] != NULL; i++)
		ft_printf("%s ", command->args[i]);
	ft_printf("}\n");
	if (alterate_filedes(command, context))
	{
		context_deep_free(&context);
		return (1);
	}
	alterate_vars(command, context);
	status = 0;
	if (!g_interrupt && command->args != NULL && command->args[0] != NULL)
	{
		jc_enqueue_job(jc_create_job(command, context, job_class));
		status = !new_context ? jc_execute_pipeline_queue() : 0;
		g_term->last_status = status;
		if (!new_context)
			jc_destroy_queue();
	}
	return (status);
}
