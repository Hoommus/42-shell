/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_mem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 18:12:11 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/27 13:31:38 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

/*
** new_token expects command to be already malloced
*/

t_token			*new_token(char *value, enum e_token_type type)
{
	t_token	*new;

	new = (t_token *)ft_memalloc(sizeof(t_token));
	new->value = value;
	new->type = type;
	return (new);
}

void			free_token(t_token *token)
{
	if (token == NULL)
		return ;
	free((void *)token->value);
	free(token);
}

void			push_token(t_token **head, t_token *token)
{
	token->next = *head;
	token->prev = NULL;
	*head = token;
}

t_token			*pop_token(t_token **head, t_token **tail)
{
	if (*head != NULL && *tail == NULL)
		return (*head);
	if (*head != NULL && *tail != NULL)
		return (*tail);
	return (NULL);
}

void			add_token(t_token **head, t_token **tail, t_token *to_add)
{
	if (*head == NULL)
		*head = to_add;
	else if (*tail == NULL)
	{
		*tail = to_add;
		to_add->prev = *head;
		(*head)->next = *tail;
	}
	else
	{
		(*tail)->next = to_add;
		to_add->prev = *tail;
		*tail = to_add;
	}
}
