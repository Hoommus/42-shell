/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plist.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 15:05:15 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/03 00:40:57 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "ft_glob.h"

static inline t_plist	*last_plist(t_plist *plist)
{
	while (plist->next)
		plist = plist->next;
	return (plist);
}

t_plist					*add_path(t_plist **plist_p, char *path)
{
	t_plist	*plist;
	t_plist	*last;

	if (!(plist = (t_plist *)malloc(sizeof(t_plist))))
		return (NULL);
	if (!(plist->path = ft_strdup(path)))
	{
		free(plist);
		return (NULL);
	}
	plist->next = NULL;
	if (!*plist_p)
		*plist_p = plist;
	else
	{
		last = last_plist(*plist_p);
		last->next = plist;
	}
	return (plist);
}

void					free_plist(t_plist *plist)
{
	t_plist	*tmp;

	while (plist)
	{
		tmp = plist;
		plist = plist->next;
		if (tmp->path)
			free(tmp->path);
		free(tmp);
	}
}
