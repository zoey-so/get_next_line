/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smilch <smilch@student.42warsaw.pl>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 15:43:59 by smilch            #+#    #+#             */
/*   Updated: 2026/06/26 22:03:33 by smilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2048
# endif

# if BUFFER_SIZE <= 0
#  error "BUFFER_SIZE must be positive"
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
void	init_node(t_rdr *node, int fd, t_rdr **r_head, t_rdr *prev_node);
t_rdr	*get_rdr(t_rdr **r_head, int fd);
char	*copy_line(char *concat_line, char *line);
char	*get_from_buf(t_rdr *rdr);
t_rdr	*read_to_buf(t_rdr *rdr, t_rdr **r_head);
void	*ft_memcpy(void *s1, const void *s2, size_t n);
size_t	ft_strlen(const char *str);
char	*ft_strchr(const char *p, int ch);
#endif
