/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acompl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 16:35:43 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/06 17:06:55 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACOMPL_H
# define ACOMPL_H

#include <limits.h>
#include "ft_printf.h"

typedef enum	e_acompl {
    ACOMPL_NOTHING,
    ACOMPL_CONTINUE,
    ACOMPL_END,
    ACOMPL_ERROR,
    ACOMPL_MAXINT = INT_MAX
}				t_acompl;

# include <glob.h>
# include <stdbool.h>

int	acompl(char *input_str, char *result_buf, size_t res_size, bool is_cmd);
int	acompl_cmd(char *input_str, char *result_buf, size_t res_size);
int	acompl_file(char *input_str, char *result_buf, size_t res_size,
		bool is_cmd);
int	filter_paths(char *pathv[], char *result_buf, size_t res_size);

#endif
