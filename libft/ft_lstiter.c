/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/01 05:07:04 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/04/01 17:08:39 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(t_list *elem))
{
	t_list	*copy;

	copy = lst;
	if (lst != NULL && f != NULL)
	{
		while (copy)
		{
			f(copy);
			copy = copy->next;
		}
	}
}
