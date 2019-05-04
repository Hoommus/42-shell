/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_andor_if.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 10:53:11 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/01 15:44:24 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

/*
** TODO: Fix global return status to match zsh's
*/

int							exec_and_if(const t_node *parent)
{
	int		status_left;
	int		status_right;

	status_right = 1;
	status_left = exec_node(parent->left);
	if (status_left == 0)
		status_right = exec_node(parent->right);
	return ((status_right));
}

int							exec_or_if(const t_node *parent)
{
	int		status_left;
	int		status_right;

	status_right = 0;
	status_left = exec_node(parent->left);
	if (status_left != 0)
		status_right = exec_node(parent->right);
	return ((status_right));
}
