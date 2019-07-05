/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_paths.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 14:52:30 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/03 01:40:21 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_glob.h"

static t_plist			*get_next(int first_is_next, t_plist **first_list,
									t_plist **second_list)
{
	t_plist	*next;

	if (first_is_next <= 0)
	{
		next = *first_list;
		*first_list = (*first_list)->next;
	}
	else
	{
		next = *second_list;
		*second_list = (*second_list)->next;
	}
	return (next);
}

static t_plist			*plist_merge(t_plist *first_list, t_plist *second_list)
{
	t_plist *new_list;
	t_plist *list_tail;

	if (!first_list)
		return (second_list);
	else if (!second_list)
		return (first_list);
	else if (!first_list && !second_list)
		return (NULL);
	new_list = get_next(ft_strcmp(first_list->path, second_list->path),
			&first_list, &second_list);
	list_tail = new_list;
	while (first_list && second_list)
	{
		list_tail->next = get_next(ft_strcmp(first_list->path,
				second_list->path), &first_list, &second_list);
		list_tail = list_tail->next;
	}
	if (first_list)
		list_tail->next = first_list;
	else if (second_list)
		list_tail->next = second_list;
	return (new_list);
}

static void				plist_merge_sort(t_plist **file_list, size_t len)
{
	t_plist	*first_part;
	t_plist	*second_part;
	t_plist	*tmp_list;
	size_t	ctr;

	if (!*file_list)
		return ;
	first_part = *file_list;
	tmp_list = *file_list;
	ctr = 0;
	while (++ctr < len / 2)
		tmp_list = tmp_list->next;
	second_part = tmp_list->next;
	tmp_list->next = NULL;
	if (first_part->next)
		plist_merge_sort(&first_part, len / 2);
	if (second_part && second_part->next)
		plist_merge_sort(&second_part, len / 2);
	*file_list = plist_merge(first_part, second_part);
}

inline static size_t	plist_len(t_plist *plist)
{
	size_t	i;

	i = 0;
	while (plist)
	{
		i++;
		plist = plist->next;
	}
	return (i);
}


int						sort_paths(t_plist **plist_p)
{
	size_t	plen;

	plen = plist_len(*plist_p);
	plist_merge_sort(plist_p, plen);
	return (0);
}
