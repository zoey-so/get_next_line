/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smilch <smilch@student.42warsaw.pl>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 15:43:54 by smilch            #+#    #+#             */
/*   Updated: 2026/06/25 18:03:00 by smilch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_reader	*get_reader(t_reader *r_head, int fd)
{
	t_reader	*new_node;
	t_reader	*node;

	node = r_head;
	while (node && node->next)
	{
		if (node->fd == fd)
			return (node);
		node = node->next;
	}
	new_node = (t_reader *)malloc(sizeof(t_reader));
	if (!new_node)
		return (NULL);
	new_node->buf = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	if (!new_node->buf)
	{
		free(new_node);
		return (NULL);
	}
	new_node->fd = fd;
	new_node->next = NULL;
	if (!node)
		r_head = new_node;
	else
		node->next = new_node;
	return (new_node);
}

char	*read_line(t_reader *reader)
{
	if (reader->buf_pos >= reader->buf_len && !reader->eof)
	{
		if (reader->buf_len == BUFFER_SIZE)
			reader->buf_pos = 1;
		reader->buf_len = read(reader->fd, reader->buf, BUFFER_SIZE);
		if (reader->buf_len <= 0)
		{
			reader->eof = 1;
			return (NULL);
		}
		reader->buf_pos = 0;
	}
	char	*line = NULL;
	int		line_len = 0;
	while (reader->buf_pos < reader->buf_len)
	{
		if (reader->buf[reader->buf_pos] == '\n')
		{
			line = (char *)malloc(sizeof(char) * (line_len + 2));
			if (!line)
				return (NULL);
			ft_memcpy(line, reader->buf + reader->buf_pos - line_len, line_len);
			line[line_len] = '\n';
			line[line_len + 1] = '\0';
			reader->buf_pos++;
			return (line);
		}
		line_len++;
		reader->buf_pos++;
	}
	return (NULL);
}

char	*copy_line(char *concat_line, char *line)
{
	char	*new_line;
	size_t	cc_len;
	size_t	l_len;

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

char	*get_from_buf(t_reader *reader)
{
	char	*line;
	size_t	len;

	len = 0;
	while (reader->buf_pos + len < reader->buf_len &&
		(reader->buf[reader->buf_pos + len] != '\n' || !reader->eof))
		len++;
	if (reader->buf[reader->buf_pos + len] == '\n')
		len++;
	line = (char *)malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	ft_memcpy(line, reader->buf + reader->buf_pos, len);
	line[len] = '\0';
	reader->buf_pos += len;
	if (reader->buf_pos >= reader->buf_len)
	{
		reader->buf_pos = 0;
		reader->buf_len = 0;
		if (reader->eof)
			free_reader(reader);
	}
	return (line);
}

void	read_to_buf(t_reader *reader)
{
	reader->buf_len = read(reader->fd, reader->buf, BUFFER_SIZE);
		if (reader->buf_len == 0)
		{
			reader->eof = 1;
			return (NULL);
		}
}

char *get_next_line(int fd)
{
	static t_reader *r_head = NULL;
	t_reader *reader;
	char	*line;
	char	*concat_line;

	reader = get_reader((r_head), fd);
	if (!reader)
		return (NULL);
	line = NULL;
	concat_line = NULL;
	while (1)
	{
		if (line)
			concat_line = copy_line(concat_line, line);
		if (reader->buf_pos < reader->buf_len)
		{
			line = get_from_buf(reader);
			if (line && (is_complete(line) || !reader))
			{
				if (concat_line)
					return (copy_line(concat_line, line));
				else
					return (line);
			}
		}
		read_to_buf(reader);
	}
}
