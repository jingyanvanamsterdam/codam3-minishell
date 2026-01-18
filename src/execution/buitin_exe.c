/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitin_exe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 15:39:19 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 15:56:25 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>		// for pid_t
#include <stdlib.h>		// for EXIT_FAILURE
#include <sys/wait.h>
#include <unistd.h>		// for STDIN_FILENO
#include <stdio.h>		// for perror()
#include "libft.h"
#include <errno.h>

// Return shell->exit; 0 for success, 1 for error (shell continues)
// close_fd will close and change it to -1;
// This function return 1 when error occurs.
int	apply_redir_parent(t_shell *shell, int savefd[2])
{
	if (shell->cmd->fd[0] != 0)
	{
		savefd[0] = dup(STDIN_FILENO);
		if (dup2(shell->cmd->fd[0], STDIN_FILENO) < 0)
		{
			shell->exit = 1;
			ft_error_printing("fail at dup2");
		}
	}
	if (shell->cmd->fd[1] != 1)
	{
		savefd[1] = dup(STDOUT_FILENO);
		if (dup2(shell->cmd->fd[1], STDOUT_FILENO) < 0)
		{
			shell->exit = 1;
			ft_error_printing("fail at dup2");
		}
	}
	close_cmd_fds(shell);
	return (shell->exit);
}

void	restore_parent_fd(int saved_stdfd[2])
{
	if (saved_stdfd[0] != -1)
	{
		dup2(saved_stdfd[0], STDIN_FILENO);
		close(saved_stdfd[0]);
	}
	if (saved_stdfd[1] != -1)
	{
		dup2(saved_stdfd[1], STDOUT_FILENO);
		close(saved_stdfd[1]);
	}
}

// This function return 1 when the command is a single builtin command
// After calling this function,
// need to check the shell->exit to see if it is 1 or 0.
void	single_builtin_handler(t_shell *shell, t_builtin type)
{
	int		saved_stdfd[2];
	t_redir	*redir;

	saved_stdfd[0] = -1;
	saved_stdfd[1] = -1;
	redir = shell->cmd->redir;
	while (redir)
	{
		if (!open_files(shell->cmd, redir, redir->type))
		{
			close_cmd_fds(shell);
			shell->exit = 1;
			return ;
		}
		redir = redir->next;
	}
	if (apply_redir_parent(shell, saved_stdfd) == 0)
		execve_builtin(shell, type, shell->cmd);
	restore_parent_fd(saved_stdfd);
}
