/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smilch <smilch@student.42warsaw.pl>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 15:43:54 by smilch            #+#    #+#             */
/*   Updated: 2026/06/25 20:45:41 by smilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	init_node(t_rdr **node, int fd, t_rdr **r_head)
{
	(*node)->fd = fd;
	(*node)->len = 0;
	(*node)->pos = 0;
	(*node)->eof = 0;
	(*node)->next = *r_head;
}

t_rdr	*get_rdr(t_rdr **r_head, int fd)
{
	t_rdr	*new_node;
	t_rdr	*node;

	write(1, "get\n", 4);
	node = *r_head;
	while (node)
	{
		if (node->fd == fd)
			return (node);
		if(node->next == *r_head)
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
	if (!node)
		*r_head = new_node;
	else
		node->next = new_node;
	init_node(&new_node, fd, r_head);
	return (new_node);
}

char	*copy_line(char *concat_line, char *line)
{
	char	*new_concat;
	size_t	cc_len;
	size_t	l_len;

	if (!concat_line && !line)
		return (NULL);
	if (!concat_line)
		return (line);
	if (!line)
		return (concat_line);
	cc_len = ft_strlen(concat_line);
	l_len = ft_strlen(line);
	new_concat = (char *)malloc(sizeof(char) * (cc_len + l_len + 1));
	if (!new_concat)
		return (NULL);
	ft_memcpy(new_concat, concat_line, cc_len);
	ft_memcpy(new_concat + cc_len, line, l_len);
	new_concat[cc_len + l_len] = '\0';
	free(concat_line);
	free(line);
	return (new_concat);
}

char	*get_from_buf(t_rdr *rdr)
{
	char	*line;
	int		len;

	len = 0;
	while (rdr->pos + len < rdr->len && rdr->buf[rdr->pos + len] != '\n')
		len++;
	if (rdr->buf[rdr->pos + len] == '\n')
		len++;
	line = (char *)malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	ft_memcpy(line, rdr->buf + rdr->pos, len);
	line[len] = '\0';
	rdr->pos += len;
	if (rdr->pos >= rdr->len)
	{
		rdr->pos = 0;
		rdr->len = 0;
	}
	return (line);
}

void	read_to_buf(t_rdr **rdr, t_rdr **r_head)
{
	t_rdr	*node;

	node = *rdr;
	(*rdr)->len = read((*rdr)->fd, (*rdr)->buf, BUFFER_SIZE);
	write(1, "read\n", 5);
	if ((*rdr)->len <= 0)
	{
		while (node->next != *rdr)
			node = node->next;
		if (node == *r_head)
			*r_head = NULL;
		node->next = (*rdr)->next;
		write(1, "free\n", 5);
		free((*rdr)->buf);
		free(*rdr);
		*rdr = NULL;
	}
}

char	*get_next_line(int fd)
{
	static t_rdr	*r_head = NULL;
	t_rdr			*rdr;
	char			*line;
	char			*concat_line;

	rdr = get_rdr(&r_head, fd);
	if (!rdr)
		return (NULL);
	line = NULL;
	concat_line = NULL;
	while (1)
	{
		concat_line = copy_line(concat_line, line);
		if (rdr->pos < rdr->len)
		{
			line = get_from_buf(rdr);
			if (line && (!rdr || ft_strchr(line, '\n')))
				return (line);
		}
		read_to_buf(&rdr, &r_head);
		if (!rdr)
			return (copy_line(concat_line, line));
	}
}
