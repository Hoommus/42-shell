/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_internal.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/01 15:02:40 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/01 17:38:50 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_FT_PRINTF_INTERNAL_H
# define FT_PRINTF_FT_PRINTF_INTERNAL_H

# include "../../libft/libft.h"
# include "ft_printf.h"

# define TRUE 1
# define FALSE 0

# define CONVERSIONS "bdiouxXDOUaAeEfFgGcCsSpn%"
# define MODIFIERS "lhjtzq"
# define SIGNED "diaAeEfFgG"
# define FLAGS "0\\L*#- +123456789."
# define NUMERIC "bBdDioOuUxXp"
# define NUMERIC_EXT "dDioOuUxXaAeEfFgG"
# define FLOATING "aAeEfFgG"
# define ITOA_UPPER "0123456789ABCDEF"
# define ITOA_LOWER "0123456789abcdef"
# define ABS(a) (a < 0 ? -(a) : a)

# define BUFFER_SIZE 2048

int				g_symbols;
long long		g_written;
int				g_output;
int				g_error;

/*
** Following chars respectively:
** '#' - alternate form
** '0' - For all conversions except n, the converted value is padded on the left
**       with zeros rather than blanks
** '-' - value is to be left adjusted on the field
** ' ' - blank before positive number of signed conversion
** '+' - always put sign before conversion
** ''' - grouped and separated by thousands
** long_afeg = 'L' mod for aAeEfFgG
*/

/*
** TODO: Make these memory-efficient by making bitwise flags in integer
*/
typedef struct	s_conv
{
	char		zero_padding;
	char		alt_form;
	char		pad_dir;
	char		space;
	char		sign;
	char		apostrophe;
	char		long_afeg;
	char		modif;
	long		precision;
	long		min_width;
	char		conv;
	char		*res;
	size_t		format_offset;
}				t_conv;

void			bufferize(char *s, long long len);
void			free_conv(t_conv **conv);
t_conv			*create_empty(void);
t_conv			*resolve(char *str, va_list *arg);
int				resolve_wildcard_or_else(t_conv *conv, char *str, va_list *list,
											int is_precision);
void			find_eval_print(char *format, va_list *list);

/*
** Parsing
*/
int				find_flags(char *str, t_conv *conv, va_list *list);
int				guess_convertion(char *str, t_conv *conv);
int				set_modifier_bits(char *str, t_conv *conv);

/*
** Number-related
*/
char			*utos_base(unsigned long long nbr, int base,
							const char *charset);
char			*itos_base(long long int nbr, int base);

/*
** Unicode
*/
char			*to_unicode(wchar_t codepoint);
char			*to_unicode_string(wchar_t *string);

/*
** Generic
*/
char			*apply_generic_width(t_conv *conv, char **str,
									size_t len, char c);
char			*apply_generic_precision(t_conv *conv, char **str, size_t len);
char			*apply_unicode_precision(t_conv *conv, char **str);
char			*apply_alternate_form_ox(t_conv *conv, char **str);
char			*apply_alternate_form_b(t_conv *conv, char **str, char delim);
char			*apply_sign(t_conv *conv, char **str, int sign);
char			*apply_space(t_conv *conv, char **str);
void			override_flags(t_conv *conv, long long int nbr);

/*
** Evals
*/
void			eval(t_conv *conv, va_list *arg);
void			eval_di(t_conv *conv, va_list *arg);
void			eval_uoxx(t_conv *conv, va_list *arg);
void			eval_b(t_conv *conv, va_list *arg);
void			eval_p(t_conv *conv, va_list *arg);
void			eval_n(t_conv *conv, va_list *arg);
void			eval_cs(t_conv *conv, va_list *arg);
void			eval_percent(t_conv *conv);
void			eval_invalid(t_conv *conv);
char			*itox(t_conv *conv, unsigned long long nbr);
char			*itob(t_conv *conv, unsigned long long nbr);
char			*itoo(t_conv *conv, unsigned long long nbr);
char			*itou(t_conv *conv, unsigned long long nbr);
char			*itod(t_conv *conv, long long int nbr);

#endif
