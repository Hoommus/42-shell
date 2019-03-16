/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_exec_simple_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 13:22:28 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/12 16:43:51 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"
#include "shell_script_parser.h"

int			exec_command(t_node *command_node)
{
	const struct s_command	*command = command_node->command;

	if (command_node->node_type != NODE_COMMAND)
	{
		ft_printf("assumed command_node is not actually command\n");
		abort();
	}


	// TODO: Solve expansions and globs
	// TODO: Remove quotes
	// TODO: Expand environment variables
	// TODO: Expand backqoutes intelligently

	// TODO: Switch context (apply all fds and variables)
	// TODO: Evaluate all redirects
	// TODO: Execute command / create and run a job
	// TODO: Close all fds and restore context
}