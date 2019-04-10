/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_andor_if.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 10:53:11 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/11 11:29:30 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

#include <assert.h>

int							exec_and_if(const t_node *parent)
{
	int		status_left;
	int		status_right;

	assert(parent && parent->node_type == NODE_AND_IF);
	status_right = 0;
	status_left = exec_node(parent->left);
	if (status_left == 0)
		status_right = exec_node(parent->right);
	return (status_left && status_right);
}

int							exec_or_if(const t_node *parent)
{
	int		status_left;
	int		status_right;

	assert(parent && parent->node_type == NODE_OR_IF);
	status_right = 0;
	status_left = exec_node(parent->left);
	if (status_left != 0)
		status_right = exec_node(parent->right);
	return (status_left == 0 || status_right == 0);
}