/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_close.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 16:34:06 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 16:35:03 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
// for the ERROR defined in parse.h
#include "libft.h"
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

void	close_fd(int *fd)
{
	if (*fd != -1 && *fd != 0 && *fd != 1 && *fd != 2)
		close(*fd);
	*fd = -1;
}

void	close_cmd_fds(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = shell->cmd;
	while (cmd)
	{
		close_fd(&(cmd->fd[0]));
		close_fd(&(cmd->fd[1]));
		close_fd(&(cmd->hdfd));
		cmd = cmd->next;
	}
}

void	close_pipes_i(t_pipe *params, int n)
{
	int	i;

	if (!params || !params->pipes)
		return ;
	i = 0;
	while (i < n)
	{
		close_fd(&(params->pipes[i][0]));
		close_fd(&(params->pipes[i][1]));
		++i;
	}
	return ;
}
