/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smilch <smilch@student.42warsaw.pl>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 15:44:01 by smilch            #+#    #+#             */
/*   Updated: 2026/06/28 15:25:31 by smilch           ###   ########.fr       */
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

// Sets attributes to initial.
// If its first node (no head) we point it to itself and set the head.
// Otherwise we point the previous one to new one and the new one to head.
void	init_node(t_rdr *node, int fd, t_rdr **r_head, t_rdr *prev_node)
{
	node->fd = fd;
	node->len = 0;
	node->pos = 0;
	node->eof = 0;
	if (!*r_head)
	{
		node->next = node;
		*r_head = node;
	}
	else
	{
		node->next = *r_head;
		prev_node->next = node;
	}
}

// Check if we have given fd opened in our nodes.
// If not - we at the same time get to the last node.
// Allocate memory for new nodem and its buffer.
// Init new node will fill the node and set its attributes.
t_rdr	*get_rdr(t_rdr **r_head, int fd)
{
	t_rdr	*new_node;
	t_rdr	*node;

	node = *r_head;
	if (BUFFER_SIZE < 0)
		return (NULL);
	while (node)
	{
		if (node->fd == fd)
			return (node);
		if (node->next == *r_head)
			break ;
		node = node->next;
	}
	new_node = (t_rdr *)malloc(sizeof(t_rdr));
	if (!new_node)
		return (NULL);
	new_node->buf = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	if (!new_node->buf)
	{
		free(new_node);
		return (NULL);
	}
	init_node(new_node, fd, r_head, node);
	return (new_node);
}
