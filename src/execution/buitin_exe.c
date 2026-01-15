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
	find_file_redir(shell->cmd);
	// dup if it is necessary
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
// After calling this function, need to check the shell->exit to see if it is 1 or 0.
int	single_builtin_handler(t_shell *shell)
{
	t_pipe	*params;
	int		command_type;
	int		saved_stdfd[2];

	params = shell->pip_param;
	saved_stdfd[0] = -1;
	saved_stdfd[1] = -1;
	if (params->cmd_count == 1 
		&& shell->cmd->cmd 
		&& shell->cmd->cmd[0])
	{
		// TODO: Can I combine the is_builtin() with execve_builtin() to reduce calling time?
		command_type = is_builtin(shell->cmd->cmd[0]);
		// TODO: If command is not a builtin, just do nothing and return 0;
		if (command_type > 0)
		{
			if (apply_redir_parent(shell, saved_stdfd) != 0)
				return (1);
			close_cmd_fds(shell);
			execve_builtin(shell, command_type, shell->cmd);
			restore_parent_fd(saved_stdfd);
			return (1);
		}
	}
	return (0);
}
