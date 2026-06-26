/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smilch <smilch@student.42warsaw.pl>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 15:43:54 by smilch            #+#    #+#             */
/*   Updated: 2026/06/26 20:52:25 by smilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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

t_rdr	*get_rdr(t_rdr **r_head, int fd)
{
	t_rdr	*new_node;
	t_rdr	*node;

	node = *r_head;
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

t_rdr	*read_to_buf(t_rdr *rdr, t_rdr **r_head)
{
	t_rdr	*prev_node;

	prev_node = rdr;
	rdr->len = read(rdr->fd, rdr->buf, BUFFER_SIZE);
	if (rdr->len <= 0) // we are in read mode so buffer is empty, if nothing to be read we free the node
	{
		if (rdr->next == rdr) // means it is the only node in the list so we set head to null
			*r_head = NULL;
		else if (rdr == *r_head)
			*r_head = rdr->next;
		while (prev_node->next != rdr) // find the previous node
			prev_node = prev_node->next;
		prev_node->next = rdr->next;
		write(1, "free\n", 5);
		free(rdr->buf);
		free(rdr);
		return (NULL);
	}
	return (rdr);
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
			if (concat_line && line && (!rdr || ft_strchr(line, '\n')))
				return (copy_line(concat_line, line));
			if (line && (!rdr || ft_strchr(line, '\n')))
				return (line);
		}
		rdr = read_to_buf(rdr, &r_head);
		if (!rdr)
		{
			return (copy_line(concat_line, line));
		}
	}
}
