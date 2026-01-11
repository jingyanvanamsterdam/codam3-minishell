#include "minishell.h"
#include <fcntl.h>		// for pid_t
#include <stdlib.h>		// for EXIT_FAILURE
#include <sys/wait.h>
#include <unistd.h>		// for STDIN_FILENO
#include <stdio.h>		// for perror()
#include "libft.h"

// Close all pipes in child process after dup2
// After dup2, stdin/stdout have copies of the pipe FDs we need, so we can close all original pipe FDs
//void	close_unused_pipes(t_pipe *params, t_cmd *cmd)
//{
//	int	j;

//	if (!params || !params->pipes)
//		return ;
//	// Close all pipe file descriptors
//	j = 0;
//	while (j < params->cmd_count - 1)
//	{
//		close(params->pipes[j][0]);
//		close(params->pipes[j][1]);
//		++j;
//	}
//	// Close the original FDs we dup2'd from (if they were not stdin/stdout)
//	// After dup2, stdin/stdout point to them, so we can close the originals
//	//if (cmd->fd[0] != 0 && cmd->fd[0] != -1)
//	//	close(cmd->fd[0]);
//	//if (cmd->fd[1] != 1 && cmd->fd[1] != -1)
//	//	close(cmd->fd[1]);

//	close_fd(&(cmd->fd[0]));
//	close_fd(&(cmd->fd[1]));
	
//	//if (stream[0] != STDIN_FILENO && stream[0] != -1)
//	//	close(stream[0]);
//	//if (stream[1] != STDOUT_FILENO && stream[1] != -1)
//	//	close(stream[1]);
//}

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

int	dup_files(t_shell *shell, int stream[2])
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
	//close_unused_pipes(shell->pip_param, cmd);
	close_cmd_fds(shell);
	close_pipes_i(shell->pip_param, shell->pip_param->cmd_count - 1);
	//close_all_fds(shell, shell->pip_param->cmd_count - 1);
	return (shell->exit);
}

void	execve_cmd(t_shell *shell, t_cmd *cmd)
{
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
				close_fd(&(cmd->fd[0]));
			cmd->fd[0] = redir->fd;
		}
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if (cmd->fd[1]!= 1)
				close_fd(&(cmd->fd[1]));
			cmd->fd[1] = redir->fd;
		}
		redir = redir->next;
	}
}

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
			ft_error_printing("dup2 input fail");
		}
	}
	if (shell->cmd->fd[1] != 1)
	{
		savefd[1] = dup(STDOUT_FILENO);
		if (dup2(shell->cmd->fd[1], STDOUT_FILENO) < 0)
		{
			shell->exit = 1;
			ft_error_printing("dup2 output fail");
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
		command_type = is_builtin(shell->cmd->cmd[0]);
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

/* Create pipes for multi-command execution, leave the pipes = NULL by deafult if single command*/
/* if (!pipes[i] || pipe(pipes[i]) == -1) 太聪明了！*/
int	create_pipes(t_shell *shell)
{
	int		i;
	t_pipe	*params;

	params = shell->pip_param;
	i = 0;
	params->pipes = malloc(sizeof(int *) * (params->cmd_count - 1));
	if (!params->pipes)
		return (ft_malloc_exe("pipes\n", shell, 0), 0);
	while (i < params->cmd_count - 1)
	{
		params->pipes[i] = malloc(sizeof(int) * 2);
		if (!params->pipes[i])
			return (ft_malloc_exe("malloc pipe: ", shell, i), 0);
		if (pipe(params->pipes[i]) == -1)
			return (ft_pipe_error(shell, "pipe: ", i), 0);
		++i;
	}
	return (1);
}

//// TODO: maybe rename it to parent_close_pipes()?
//void	parent_close_file(t_shell *shell, int i)
//{
//	int	count;
//	int **pipes;

//	count = shell->pip_param->cmd_count;
//	pipes = shell->pip_param->pipes;
//	if (i != 0 && pipes)
//		close(pipes[i - 1][0]);
//	else if (shell->cmd->fd[0] != -1)
//		close(shell->cmd->fd[0]);
//	if (i < count - 1 && pipes)
//		close(pipes[i][1]);
//	else if (shell->cmd->fd[1] != -1)
//		close(shell->cmd->fd[1]);
//}

void	run_child_process(t_shell *shell, t_cmd *cmd, int stream[2])
{
	int	command;

	sig_exe_child();
	if (cmd->cmd)
	{
		command = is_builtin(cmd->cmd[0]);
		if (dup_files(shell, stream) == 0)
		{
			if (command > 0)
				execve_builtin(shell, command, cmd);
			else
				execve_cmd(shell, cmd);
		}
	}
	// TODO: need to close pipes? As pipes are malloced in parent process：pipes已经在dup files的时候全都关闭了，只需要free就可以; free_pipes 在free_shell 里；
	// TODO: need to free shell? 我觉得的是的，因为child process继承了parent process的所有。所以退出之前应该free
	ft_process_exit(shell, false);
}

	// Parent closes all pipes after forking all children
	// Each child has its own copies via dup2, so parent doesn't need them
	//close_pipes(params);
int	create_process(t_shell *shell)
{
	t_pipe	*params;
	t_cmd	*cmd;
	int		i;
	int		stream[2];

	params = shell->pip_param;
	params->pids = ft_calloc(params->cmd_count, sizeof(pid_t));
	if (!params->pids)
		return (ft_malloc_exe("pid", shell, params->cmd_count - 1), 0);
	cmd = shell->cmd;
	i = 0;
	while (i < params->cmd_count)
	{
		find_file_redir(cmd);
		setup_stream(stream, cmd, i, shell);
		params->pids[i] = fork();
		if (params->pids[i] < 0)
			return (ft_pipe_error(shell, "fork", params->cmd_count - 1), 0);
		else if (params->pids[i] == 0)
			run_child_process(shell, cmd, stream);
		cmd = cmd->next;
		++i;
	}
	close_cmd_fds(shell);
	close_pipes_i(shell->pip_param, params->cmd_count - 1);
	return (1);
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
				shell->exit = WEXITSTATUS(status);		// TODO: need JD to confirm should be shell->exit. bc we have reset  shell after execution	是的吧，run_child_process 返回一个exit code；然后这个shell->exit就是此次运行的最后一个command返回的exit code；execute（) 结束后，shell->prevexit = shell->exit. shell->exit = 0；
			else if (WIFSIGNALED(status))
				shell->exit = 128 + WTERMSIG(status);
		}
		++i;
	}
	free_pip_param(shell, shell->pip_param->cmd_count - 1);
}

// After calling this function, need to check the shell->exit to see if it is 1 or 0.
void	executor(t_shell *shell)
{
	shell->pip_param = malloc(sizeof(t_pipe));
	if (!shell->pip_param)
	{
		ft_malloc_error("executuion", shell);
		close_cmd_fds(shell);
		return ;
	}
	shell->pip_param->cmd_count = count_cmd(shell->cmd);
	shell->pip_param->pids = NULL;
	shell->pip_param->pipes = NULL;
	if (single_builtin_handler(shell))
	{
		close_cmd_fds(shell);
		free_pip_param(shell, 0);
		return ;
	}
	if (shell->pip_param->cmd_count > 1 && !create_pipes(shell))
		return ;
	if (!create_process(shell))
		return ;
	wait_handler(shell);
}
