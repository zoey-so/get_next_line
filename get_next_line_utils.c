/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smilch <smilch@student.42warsaw.pl>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 15:44:01 by smilch            #+#    #+#             */
/*   Updated: 2026/06/25 19:29:10 by smilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_memcpy(void *s1, const void *s2, size_t n)
{
	const char	*src;
	char		*dst;

	src = s2;
	dst = s1;
	while (n-- > 0)
		*dst++ = *src++;
	return (s1);
}

size_t	ft_strlen(const char *str)
{
	const char	*s_cp;

	s_cp = str;
	while (*str)
		str++;
	return (str - s_cp);
}

char	*ft_strchr(const char *p, int ch)
{
	if (*p == (char)ch)
		return ((char *)p);
	while (*p++)
	{
		if (*p == (char)ch)
			return ((char *)p);
	}
	return (NULL);
}
