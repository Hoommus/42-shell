/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_exec_simple_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 13:22:28 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/27 13:29:07 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"
#include "shell_script_parser.h"

int							simple_command_execute(t_node *command_node)
{
	const struct s_command	*command = (struct s_command *)command_node->value;

	// TODO: Solve expansions and globs
	// TODO: Remove quotes
	// TODO: Expand environment variables
	// TODO: Expand backqoutes intelligently

	return (1 /* TODO: process return code */);
}
