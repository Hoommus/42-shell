/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acompl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 16:35:43 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/10 13:18:22 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACOMPL_H
# define ACOMPL_H

# include <glob.h>
# include <stdbool.h>
# include <limits.h>

typedef enum	e_acompl {
	ACOMPL_NOTHING,
	ACOMPL_CONTINUE,
	ACOMPL_END,
	ACOMPL_ERROR,
	ACOMPL_MAXINT = INT_MAX
}				t_acompl;

int				acompl(char *input_str, char *result_buf, size_t res_size,
	bool is_cmd);
int				acompl_cmd(char *input_str, char *result_buf, size_t res_size);
int				acompl_file(char *input_str, char *result_buf, size_t res_size,
	bool is_cmd);

int				get_builtv(char *input_str, char *builtv[]);
int				get_builtc(char *input_str);

int				get_aliasv(char *input_str, char *aliasv[]);
int				get_aliasc(char *input_str);

int				get_hashv(char *input_str, char *hashv[]);
int				get_hashc(char *input_str);

int				filter_paths(char *pathv[], char *result_buf, size_t res_size);

#endif
