/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:45 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/31 14:45:45 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_array(char **array)
{
	int		i;

	i = 0;
	while (array[i])
	{
		chfree(array[i]);
		i++;
	}
	free(array);
}

void	chfree(void *obj)
{
	if (obj != NULL)
		free(obj);
}

void	chfree_n(int n, ...)
{
	va_list		list;
	void		*dummy;

	va_start(list, n);
	while (n)
	{
		if ((dummy = va_arg(list, void *)))
			free(dummy);
		n--;
	}
	va_end(list);
}
