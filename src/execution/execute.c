#include "minishell.h"
#include <fcntl.h>		// for pid_t
#include <stdlib.h>		// for EXIT_FAILURE
#include <sys/wait.h>
#include <unistd.h>		// for STDIN_FILENO
#include <stdio.h>		// for perror()
#include "libft.h"

void	find_file_redir(t_cmd *cmd, int *file)
{
	t_redir *redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
			file[0] = redir->fd;
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
			file[1] = redir->fd;
		redir = redir->next;
	}
}

// Return 0 for success, 1 for error (shell continues)
// This function return 1 when error occurs.
int	apply_redirection_parent(t_redir *r, int *saved_stdfd)
{// TODO: In this function, need to find the last redirection and dup2() it.
	// int	fds[2];

	// fds[0] = STDIN_FILENO;
	// fds[1] = STDOUT_FILENO;
	// find_file_redir(r, fds);
	while (r)
	{
		if (r->fd < 0)
		{
			perror(r->file);		// TODO: use a function metioned by JD?
			return (1);
		}
		if (r->type == REDIR_IN || r->type == HEREDOC)
		{
			if (saved_stdfd[0] == -1)
				saved_stdfd[0] = dup(STDIN_FILENO);
			if (dup2(r->fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				return (1);
			}
		}
		else if (r->type == APPEND || r->type == REDIR_OUT)
		{
			if (saved_stdfd[1] == -1)
				saved_stdfd[1] = dup(STDOUT_FILENO);
			if (dup2(r->fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				return (1);
			}
		}
		close(r->fd);		// all fd are closed unless early return due to error.
		r = r->next;
	}
	return (0);
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
	saved_stdfd[0] = -1;;
	saved_stdfd[1] = -1;
	if (params->cmd_count == 1 && shell->cmd->cmd && shell->cmd->cmd[0])
	{
		command_type = is_builtin(shell->cmd->cmd[0]);
		if (command_type > 0)
		{
			if (shell->cmd->redir)
			{
				if (apply_redirection_parent(shell->cmd->redir, saved_stdfd) != 0)
				{
					shell->exit = 1;
					return (1);
				}
			}
			execve_builtin(shell, command_type, shell->cmd);
			restore_parent_fd(saved_stdfd);
			return (1);
		}
	}
	return (0);
}

// After calling this function, need to check the shell->exit to see if it is 1 or 0.
void	executor(t_shell *shell)
{
	t_pipe	params;

	params = (t_pipe){0};
	params.cmd_count = count_cmd(shell->cmd);
	shell->pip_param = &params;

	if (single_builtin_handler(shell))
		return ;

	// 
	// if (params.cmd_count > 1)
	// 	pipes_initializer(&params);
	// else
	// 	params.pipes = NULL;

	// pipe_executor(shell);		// TODO: This function need to tell the difference between params->pipes = NULL or params->pipes != NULL
	// wait_handler(shell, &params);
}
