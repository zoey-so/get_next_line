/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smilch <smilch@student.42warsaw.pl>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 15:43:59 by smilch            #+#    #+#             */
/*   Updated: 2026/06/25 19:54:45 by smilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2048
# endif

typedef struct s_rdr
{
	char		*buf;
	int			fd;
	int			len;
	int			pos;
	int			eof;
	struct s_rdr	*next;
}	t_rdr;

char	*get_next_line(int fd);
void	*ft_memcpy(void *s1, const void *s2, size_t n);
size_t	ft_strlen(const char *str);
char	*ft_strchr(const char *p, int ch);
#endif
