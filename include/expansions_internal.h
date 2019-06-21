/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_internal.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:19:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/21 13:56:25 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSIONS_INTERNAL_H
# define EXPANSIONS_INTERNAL_H

struct						s_escape_map
{
	const char	what;
	const char	where;
};

struct						s_expansion
{
	const char *const	starting_point;
	char				*(*const function)(char *string);
};

char						*expand_tilde(char *str);
char						*expand_vars(char *str);
char						*expand_escaped(char *str);
char						*expand_quotes(char *str);
char						*expand_history(char *str);

#endif
