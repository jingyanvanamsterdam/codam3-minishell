/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 15:39:53 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 18:10:34 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include "libft.h"
#include <errno.h>

static int	open_redir_fd(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;
	int		count;

	redir = cmd->redir;
	count = shell->pip_param->cmd_count;
	while (redir)
	{
		if (!open_files(cmd, redir, redir->type))
		{
			close_cmd_fds(shell);
			close_pipes_i(shell->pip_param, count - 1);
			return (0);
		}
		redir = redir->next;
	}
	return (1);
}

int	setup_stream(int stream[2], t_cmd *cmd, int i, t_shell *shell)
{
	int	count;
	int	**pipes;

	count = shell->pip_param->cmd_count;
	pipes = shell->pip_param->pipes;
	if (!open_redir_fd(cmd, shell))
	{
		shell->exit = 1;
		return (0);
	}
	if (i != 0 && cmd->fd[0] == 0 && pipes)
		stream[0] = pipes[i - 1][0];
	else
		stream[0] = cmd->fd[0];
	if (i < (count - 1) && cmd->fd[1] == 1 && pipes)
		stream[1] = pipes[i][1];
	else
		stream[1] = cmd->fd[1];
	return (1);
}

int	dup_files(t_shell *shell, int stream[2])
{
	if (dup2(stream[0], STDIN_FILENO) < 0)
	{
		shell->exit = 1;
		ft_error_printing("fail at dup2 in");
	}
	if (dup2(stream[1], STDOUT_FILENO) < 0)
	{
		shell->exit = 1;
		ft_error_printing("fail at dup2 out");
	}
	close_cmd_fds(shell);
	close_pipes_i(shell->pip_param, shell->pip_param->cmd_count - 1);
	return (shell->exit);
}
