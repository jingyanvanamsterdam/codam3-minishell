/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jdong <jdong@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/15 14:25:01 by jdong         #+#    #+#                 */
/*   Updated: 2026/01/13 23:32:44 by jingyandong   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

char	*combine_buffers(char *buffer, char *temp_buf, size_t temp_len)
{
	char	*joinedstr;
	size_t	size;

	size = ft_strlen(buffer) + temp_len + 1;
	joinedstr = ft_calloc(size, 1);
	if (!joinedstr)
		return (free(buffer), NULL);
	ft_strlcpy(joinedstr, buffer, size);
	ft_strlcat(joinedstr, temp_buf, size);
	free(buffer);
	return (joinedstr);
}

char	*get_contents(char *buffer, int fd)
{
	char	*temp_buf;
	ssize_t	byte_read;

	if (!buffer)
	{
		buffer = ft_calloc(BUFFER_SIZE + 1, 1);
		if (!buffer)
			return (NULL);
	}
	byte_read = 1;
	temp_buf = ft_calloc(BUFFER_SIZE + 1, 1);
	if (!temp_buf)
		return (free(buffer), NULL);
	while (byte_read > 0)
	{
		byte_read = read(fd, temp_buf, BUFFER_SIZE);
		if (byte_read == -1)
			return (free(temp_buf), free(buffer), NULL);
		temp_buf[byte_read] = '\0';
		buffer = combine_buffers(buffer, temp_buf, (size_t)byte_read);
		if (!buffer || ft_strchr(temp_buf, '\n'))
			break ;
	}
	return (free(temp_buf), buffer);
}

char	*get_curr_line(char *buffer)
{
	char	*line;
	size_t	len;
	size_t	i;

	i = 0;
	len = 0;
	if (!buffer[i])
		return (NULL);
	while (buffer[len] && buffer[len] != '\n')
		len++;
	line = ft_calloc(len + 2, 1);
	if (!line)
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i] = '\n';
	return (line);
}

char	*new_line_pointer(char	*buffer)
{
	char	*line;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
		return (free(buffer), NULL);
	line = ft_calloc(ft_strlen(buffer) - i + 1, 1);
	if (!line)
		return (free(buffer), NULL);
	i++;
	while (buffer[i])
		line[j++] = buffer[i++];
	free(buffer);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = get_contents(buffer, fd);
	if (!buffer)
		return (NULL);
	line = get_curr_line(buffer);
	if (!line)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	buffer = new_line_pointer(buffer);
	return (line);
}
