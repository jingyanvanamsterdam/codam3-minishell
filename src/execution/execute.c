#include "minishell.h"
#include <fcntl.h>		// for pid_t
#include <stdlib.h>		// for EXIT_FAILURE
#include <sys/wait.h>
#include <unistd.h>		// for STDIN_FILENO
#include <stdio.h>		// for perror()
#include "libft.h"

// Close all pipes in child process after dup2
// After dup2, stdin/stdout have copies of the pipe FDs we need, so we can close all original pipe FDs
void	close_unused_pipes(t_pipe *params, t_cmd *cmd)
{
	int	j;

	if (!params || !params->pipes)
		return ;
	// Close all pipe file descriptors
	j = 0;
	while (j < params->cmd_count - 1)
	{
		close(params->pipes[j][0]);
		close(params->pipes[j][1]);
		++j;
	}
	// Close the original FDs we dup2'd from (if they were not stdin/stdout)
	// After dup2, stdin/stdout point to them, so we can close the originals
	if (cmd->fd[0] != STDIN_FILENO && cmd->fd[0] != -1)
		close(cmd->fd[0]);
	if (cmd->fd[1] != STDOUT_FILENO && cmd->fd[1] != -1)
		close(cmd->fd[1]);
	
	//if (stream[0] != STDIN_FILENO && stream[0] != -1)
	//	close(stream[0]);
	//if (stream[1] != STDOUT_FILENO && stream[1] != -1)
	//	close(stream[1]);
}

void	setup_stream(int stream[2], t_cmd *cmd, int i, t_shell *shell)
{
	int	count;
	int	**pipes;

	count = shell->pip_param->cmd_count;
	pipes = shell->pip_param->pipes;
	// Set input stream
	if (i != 0 && cmd->fd[0] == 0 && pipes)
		stream[0] = pipes[i - 1][0];
	else 
		stream[0] = cmd->fd[0];
	// Set output stream
	if (i < (count - 1) && cmd->fd[1] == 1 && pipes)
		stream[1] = pipes[i][1];
	else
		stream[1] = cmd->fd[1];
}

int	dup_files(t_shell *shell, t_cmd *cmd, int stream[2])
{
	if (dup2(stream[0], STDIN_FILENO) < 0)
	{
		shell->exit = 1;
		ft_error_printing("dup2 input fail");
	}
	if (dup2(stream[1], STDOUT_FILENO) < 0)
	{
		shell->exit = 1;
		ft_error_printing("dup2 output fail");
	}
	// Close all pipes - after dup2, stdin/stdout have copies of what we need
	close_unused_pipes(shell->pip_param, cmd);
	//// Close original redirect FDs if they're not the same as what we dup2'd
	//if (cmd->fd[0] != -1 && cmd->fd[0] != stream[0])
	//	close(cmd->fd[0]);
	//if (cmd->fd[1] != -1 && cmd->fd[1] != stream[1])
	//	close(cmd->fd[1]);
	return (shell->exit);
}

void	execve_cmd(t_shell *shell, t_cmd *cmd)
{
	//char	**enviorn;
	if (!cmd->cmd)
	{
		shell->exit = 0;
		return ;
	}
	if (!cmd->path)
	{
		shell->exit = EXIT_NOCMD;
		return ;
	}
	if (execve(cmd->path, cmd->cmd, env_to_array(shell->env_lst)) == -1)
	{
		ft_error_printing(cmd->cmd[0]);
		shell->exit = EXIT_CMD_NOEXC;
	}
}

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
int	apply_redir_parent(t_shell *shell, int savefd[2])
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
		savefd[1] = dup(STDOUT_FILENO);
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
// int	apply_redirection_parent(t_redir *r, int *saved_stdfd)
// {// TODO: In this function, need to find the last redirection and dup2() it.
// 	// int	fds[2];

// 	// fds[0] = STDIN_FILENO;
// 	// fds[1] = STDOUT_FILENO;
// 	//
// 	// example code: check out apply_redir_parent_example()
// 	// find_file_redir(r, fds);
// 	while (r)
// 	{
// 		if (r->fd < 0)
// 		{
// 			perror(r->file);		// TODO: use a function metioned by JD?
// 			return (1);
// 		}
// 		if (r->type == REDIR_IN || r->type == HEREDOC)
// 		{
// 			if (saved_stdfd[0] == -1)
// 				saved_stdfd[0] = dup(STDIN_FILENO);
// 			if (dup2(r->fd, STDIN_FILENO) == -1)
// 			{
// 				perror("dup2");
// 				return (1);
// 			}
// 		}
// 		else if (r->type == APPEND || r->type == REDIR_OUT)
// 		{
// 			if (saved_stdfd[1] == -1)
// 				saved_stdfd[1] = dup(STDOUT_FILENO);
// 			if (dup2(r->fd, STDOUT_FILENO) == -1)
// 			{
// 				perror("dup2");
// 				return (1);
// 			}
// 		}
// 		close(r->fd);		// all fd are closed unless early return due to error.
// 		r = r->next;
// 	}
// 	return (0);
// }

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
			 */
			if (apply_redir_parent(shell, saved_stdfd) != 0)
				return (shell->exit);
			execve_builtin(shell, command_type, shell->cmd);
			restore_parent_fd(saved_stdfd);
			return (1);
		}
	}
	return (0);
}

/* Create pipes for multi-command execution, leave the pipes = NULL by deafult if single command*/
/* if (!pipes[i] || pipe(pipes[i]) == -1) 太聪明了！*/
void	create_pipes(t_shell *shell)
{
	int	i;
	t_pipe	*params;

	params = shell->pip_param;
	if (params->cmd_count <= 1)
	{
		params->pipes = NULL;
		return ;
	}
	i = 0;
	params->pipes = malloc(sizeof(int *) * (params->cmd_count - 1));
	if (!params->pipes)
		ft_malloc_failure("pipe malloc failure\n", shell);
	while (i < params->cmd_count - 1)
	{
		params->pipes[i] = malloc(sizeof(int) * 2);
		if (!params->pipes[i] || pipe(params->pipes[i]) == -1)
		{
			close_pipes_i(shell->pip_param, i);
			ft_pipe_error(shell, "pipe: ", i);
		}
		++i;
	}
}

// TODO: maybe rename it to parent_close_pipes()?
void	parent_close_file(t_shell *shell, int i)
{
	int	count;
	int **pipes;

	count = shell->pip_param->cmd_count;
	pipes = shell->pip_param->pipes;
	if (i != 0 && pipes)
		close(pipes[i - 1][0]);
	else if (shell->cmd->fd[0] != -1)
		close(shell->cmd->fd[0]);
	if (i < count - 1 && pipes)
		close(pipes[i][1]);
	else if (shell->cmd->fd[1] != -1)
		close(shell->cmd->fd[1]);
}

void	run_child_process(t_shell *shell, t_cmd *cmd, int stream[2])
{
	int	command;

	//setup_child_signal();
	if (cmd->cmd)
	{
		command = is_builtin(cmd->cmd[0]);
		if (dup_files(shell, cmd, stream) == 0)
		{
			if (command > 0)
				execve_builtin(shell, command, cmd);
			else
				execve_cmd(shell, cmd);
		}
	}
	// TODO: need to close pipes? As pipes are malloced in parent process
	free_pipes(shell->pip_param);
	// TODO: need to free shell?
	exit(shell->exit);
}

void	create_process(t_shell *shell)
{
	t_pipe	*params;
	t_cmd	*cmd;
	int		i;
	int		stream[2];

	params = shell->pip_param;
	params->pids = ft_calloc(params->cmd_count, sizeof(pid_t));
	cmd = shell->cmd;
	i = 0;
	while (i < params->cmd_count)
	{
		find_file_redir(cmd);
		setup_stream(stream, cmd, i, shell);
		params->pids[i] = fork();
		if (params->pids[i] < 0)
			ft_pipe_error(shell, "sh: fork", i);
		else if (params->pids[i] == 0)
			run_child_process(shell, cmd, stream);
		// Parent doesn't close pipes here - wait until all children are forked
		cmd = cmd->next;
		++i;
	}
	// Parent closes all pipes after forking all children
	// Each child has its own copies via dup2, so parent doesn't need them
	close_pipes(params);
}

void	wait_handler(t_shell *shell)
{
	int		i;
	int		status;
	t_pipe	*params;

	params = shell->pip_param;
	i = 0;
	while (i < params->cmd_count)
	{
		waitpid(params->pids[i], &status, 0);
		if (i == params->cmd_count - 1)
		{
			if (WIFEXITED(status))
				shell->exit = WEXITSTATUS(status);		// TODO: need JD to confirm should be shell->exit. bc we have reset shell after execution
			else if (WIFSIGNALED(status))
				shell->exit = 128 + WTERMSIG(status);
		}
		++i;
	}
	free(params->pids);
	params->pids = NULL;
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
	create_pipes(shell);
	create_process(shell);
	wait_handler(shell);

	// 
	// if (params.cmd_count > 1)
	// 	pipes_initializer(&params);
	// else
	// 	params.pipes = NULL;

	// pipe_executor(shell);		// TODO: This function need to tell the difference between params->pipes = NULL or params->pipes != NULL
	// wait_handler(shell, &params);
}

void	close_pipes(t_pipe *params)
{
	int	i;

	if (!params || !params->pipes)
		return ;
	i = 0;
	while (i < params->cmd_count - 1)
	{
		close(params->pipes[i][0]);
		close(params->pipes[i][1]);
		++i;
	}
	return ;
}

void	close_pipes_i(t_pipe *params, int n)
{
	int	i;

	if (!params || !params->pipes)
		return ;
	i = 0;
	while (i < n)
	{
		close(params->pipes[i][0]);
		close(params->pipes[i][1]);
		++i;
	}
	return ;
}

