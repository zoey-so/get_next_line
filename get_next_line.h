/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smilch <smilch@student.42warsaw.pl>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 15:43:59 by smilch            #+#    #+#             */
/*   Updated: 2026/06/25 16:05:59 by smilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2048
# endif

typedef struct s_reader
{
	char		*buf;
	int			fd;
	int			buf_len;
	int			buf_pos;
	int			eof;
	t_reader	*next;
}	t_reader;

char *get_next_line(int fd);
#endif
