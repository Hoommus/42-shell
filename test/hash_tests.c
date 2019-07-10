/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 10:04:29 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/06 18:17:12 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "shell_hashtable.h"
#include "ft_printf.h"
#include "get_next_line.h"

#define HASH_TABLE_SIZE 2
#define BUF_SIZE 16

void	hash_dump(t_hash_tab *hash_table)
{
	size_t		i;
	t_hash_node	*cur_node;

	i = 0;
	while (i < hash_table->size)
	{
		ft_printf("%zu)\n", i);
		if (!(cur_node = hash_table->htab[i]))
			ft_printf("    EMPTY\n");
		else
		{
			while (cur_node)
			{
				ft_printf("    %s -> %s\n", cur_node->key, cur_node->value);
				cur_node = cur_node->next;
			}
		}
		i++;
	}
}

int			main(int argc, char *argv[])
{
	t_hash_tab	*table;
	char		*cmd, buf[BUF_SIZE];
	int			tab_size;

	if (argc != 2)
	{
		ft_printf("wrong params count\n");
		return (1);
	}
	if (!(tab_size = ft_atoi(argv[1])))
	{
		ft_printf("wrong tabsize value\n");
		return (1);
	}
	hash_init(&table, tab_size);
	ft_printf("> ");
	while (get_next_line(STDIN_FILENO, &cmd))
	{
		if (!ft_strcmp(cmd, "dump"))
			hash_dump(table);
		else if (!ft_strcmp(cmd, "exit"))
			break ;
		else if (!ft_strcmp(cmd, "add"))
		{
			char	*key, *value;

			ft_printf("key: ");
			if (!get_next_line(STDIN_FILENO, &key))
				continue ;
			ft_printf("value: ");
			if (!get_next_line(STDIN_FILENO, &value))
			{
				free(key);
				continue; ;
			}
			if (ft_strlen(key) && ft_strlen(value) &&  ft_strlen(value) < BUF_SIZE)
				hash_add(table, key, value);
			else
				ft_printf("params can't be empty!\n");
			free(key);
			free(value);
		}
		else if (!ft_strcmp(cmd, "get"))
		{
			char	*key;

			ft_printf("key: ");
			if (!get_next_line(STDIN_FILENO, &key))
				continue ;
			if (ft_strlen(key))
			{
				buf[0] = '\0';
				hash_get(table, key, buf, BUF_SIZE);
				ft_printf("value: %s\n", buf);
			}
			else
				ft_printf("param can't be empty!\n");
			free(key);
		}
		else
			ft_printf("undefined cmd\n");
		ft_printf("> ");
		free(cmd);
	}
	hash_free(table);
}