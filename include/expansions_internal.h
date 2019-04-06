/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_internal.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:19:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/06 16:40:43 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSIONS_H
# define EXPANSIONS_H

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
char						*expand_escaped(char *string);
char						*expand_quote(char *string);
char						*expand_dquote(char *string);

#endif
