/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 16:24:00 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 16:30:02 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>

int	open_outfile(char *file)
{
	int	fd;

	fd = -1;
	if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
		return (fd);
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	return (fd);
}

int	output_append(char *file)
{
	int	fd;

	fd = -1;
	if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
		return (fd);
	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}
