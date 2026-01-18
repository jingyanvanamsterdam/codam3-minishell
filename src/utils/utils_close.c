/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils_close.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kuyu <kuyu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/01/18 16:34:06 by kuyu          #+#    #+#                 */
/*   Updated: 2026/01/18 17:54:38 by jdong         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "libft.h"
#include <unistd.h>

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
