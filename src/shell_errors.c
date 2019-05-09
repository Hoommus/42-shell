/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 14:32:47 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/08 18:47:29 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

char	*ft_replace_all(const char *str, const char *needle,
						const char *replacement)
{
	char		*strstr;
	char		*final;
	size_t		len;
	size_t		needle_len;
	size_t		offset;

	if (needle == NULL || replacement == NULL || ft_strlen(needle) == 0)
		return (ft_strdup(str));
	else if (str == NULL)
		return (NULL);
	len = ft_strlen(str);
	needle_len = ft_strlen(needle);
	if (ft_strlen(replacement) > needle_len)
		final = ft_strnew((1 + len / needle_len) * ft_strlen(replacement));
	else
		final = ft_strnew(len);
	offset = 0;
	while (offset < len && (strstr = ft_strstr(str + offset, needle)))
	{
		ft_strncat(final, str + offset, ABS(str - strstr + (ssize_t)offset));
		ft_strncat(final, replacement, ft_strlen(replacement));
		offset += ABS(str - strstr + (ssize_t)offset) + needle_len;
	}
	return (final);
}

int		puterr(const char *format, const char *cause)
{
	char	*swap;

	swap = ft_replace_all(cause, "\n", "\\n");
	ft_dprintf(2, format, swap);
	free(swap);
	return (0);
}
