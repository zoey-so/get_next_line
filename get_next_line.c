/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smilch <smilch@student.42warsaw.pl>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 15:43:54 by smilch            #+#    #+#             */
/*   Updated: 2026/06/26 22:14:55 by smilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Handling the concatenation of many buffer reads without newline.
// This function can return NULL if its called after read of 0.
// If its only concatenated string or line we return what we have.
// Otherwise we allocate memory for both and free what was copied.
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

// Getting a line from node's buffer.
// Calculate len of the line or of everything in buff if no \n
// Allocate for the string with null termination.
// Copy from buffer to string, terminate.
// Adjust the position index if all was read set the len and pos to 0.
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

// Reading to buffer - only if buffer is fully deplated.
// If read is <0 eof or error, since the buffer is empty we free the node.
// If it's the only node (point to itself) we set head to NULL.
// If it's not the only node, but its head node - we set head to next node.
// We set previous node to currents node next.
// Finally we free the buffer and node itself.
// No lose ends.
t_rdr	*read_to_buf(t_rdr *rdr, t_rdr **r_head)
{
	t_rdr	*prev_node;

	prev_node = rdr;
	rdr->len = read(rdr->fd, rdr->buf, BUFFER_SIZE);
	if (rdr->len <= 0)
	{
		if (rdr->next == rdr)
			*r_head = NULL;
		else if (rdr == *r_head)
			*r_head = rdr->next;
		while (prev_node->next != rdr)
			prev_node = prev_node->next;
		prev_node->next = rdr->next;
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
