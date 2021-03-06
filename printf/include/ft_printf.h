/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 12:16:11 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/01 15:02:18 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stddef.h>
# include <stdarg.h>
# include <stdlib.h>
# include <limits.h>

# ifndef __printflike
#  define __printflike(a, b) __attribute__((format(printf, a, b)))
# endif

# define PRTF int ft_printf(const char *restrict format, ...) __printflike(1, 2)
# define DRTF int ft_dprintf(int fd, const char *format, ...) __printflike(2, 3)

PRTF;
DRTF;

/*
** Printf variations
*/
int				ft_printf_fd(int fd, const char *restrict format, va_list *lst);

#endif
