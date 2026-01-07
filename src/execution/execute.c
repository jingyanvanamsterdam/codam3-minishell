#include "minishell.h"
#include <fcntl.h>		// for pid_t
#include <stdlib.h>		// for EXIT_FAILURE
#include <sys/wait.h>
#include <unistd.h>		// for STDIN_FILENO
#include <stdio.h>		// for perror()
#include "libft.h"

/** The function close the previous redirect fd if there is any. This function can be used for execute single cmd or multiple cmds*/
void	find_file_redir(t_cmd *cmd)
{
	t_redir *redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			if (cmd->fd[0] > 0)
				close(cmd->fd[0]);
			cmd->fd[0] = redir->fd;
		}
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if (cmd->fd[1] != -1 && cmd->fd[1]!= 1)
				close(cmd->fd[1]);
			cmd->fd[1] = redir->fd;
		}
		redir = redir->next;
	}
}

//void	find_file_redir(t_cmd *cmd, int *file)
//{
//	t_redir *redir;

//	redir = cmd->redir;
//	while (redir)
//	{
//		if (redir->type == REDIR_IN || redir->type == HEREDOC)
//			file[0] = redir->fd;
//		else if (redir->type == REDIR_OUT || redir->type == APPEND)
//			file[1] = redir->fd;
//		redir = redir->next;
//	}
//}

// Return shell->exit; 0 for success, 1 for error (shell continues)
// This function return 1 when error occurs.
int	apply_redir_parent_example(t_shell *shell, int savefd[2])
{
	// since we only do this because there is only one cmd and it is builtin. we don't need to loop through t_cmd linked list

	find_file_redir(shell->cmd);
	// dup if it is necessary
	if (shell->cmd->fd[0] != 0)
	{
		savefd[0] = dup(STDIN_FILENO);
		if (dup2(shell->cmd->fd[0], STDIN_FILENO) < 0)
		{
			shell->exit = 1;
			ft_error_printing("dup2 input fail");
		}
		if (shell->cmd->fd[0] != -1)
			close(shell->cmd->fd[0]);
	}
	if (shell->cmd->fd[1] != 1)
	{
		savefd[1] = dup(STDIN_FILENO);
		if (dup2(shell->cmd->fd[1], STDOUT_FILENO) < 0)
		{
			shell->exit = 1;
			ft_error_printing("dup2 output fail");
		}
		if (shell->cmd->fd[1] != -1)
			close(shell->cmd->fd[1]);
	}
	return (shell->exit);
}

// Return 0 for success, 1 for error (shell continues)
// This function return 1 when error occurs.
int	apply_redirection_parent(t_redir *r, int *saved_stdfd)
{// TODO: In this function, need to find the last redirection and dup2() it.
	// int	fds[2];

	// fds[0] = STDIN_FILENO;
	// fds[1] = STDOUT_FILENO;
	//
	// example code: check out apply_redir_parent_example()
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
			// no need to check redir exist, if it exists, cmd->fd will be dup2; otherwise it will be kept as default(0 and 1), then saved_fd will be kept as -1;
			// can call apply_redir_parent_example(shell, saved_stdfd);
			/**
			 * if (apply_redir_parent_example(shell, saved_stdfd) != 0)
			 * 	return (shell->exit);
			 * execve_builtin();
			 * restore_parent_fd(saved_stdfd);
			 * 
			 */
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
	//else
	//{
	//	create_pipes(shell);
	//	create_process(shell);
	//	wait_handler(shell);
	//}

	// 
	// if (params.cmd_count > 1)
	// 	pipes_initializer(&params);
	// else
	// 	params.pipes = NULL;

	// pipe_executor(shell);		// TODO: This function need to tell the difference between params->pipes = NULL or params->pipes != NULL
	// wait_handler(shell, &params);
}
