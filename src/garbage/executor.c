#include "minishell.h"
#include <fcntl.h>		// for pid_t
#include <stdlib.h>		// for EXIT_FAILURE
#include <sys/wait.h>
#include <unistd.h>		// for STDIN_FILENO
#include <stdio.h>		// for perror()
#include "libft.h"



void	pipes_initializer(t_pipe *params)
{
	int	i;

	params->pipes = malloc((params->cmd_count - 1) * sizeof(int *));
	if (!params->pipes)
	{
		perror("pipe array allocation");
		exit(1);
	}
	i = 0;
	while (i < params->cmd_count - 1)
	{
		params->pipes[i] = malloc(2 * sizeof(int));
		if (!params->pipes[i])
		{
			// Free already allocated pipes
			while (--i >= 0)
				free(params->pipes[i]);
			free(params->pipes);
			perror("pipe array allocation");
			exit(1);
		}		// TODO: replace it with proper exiter
		++i;
	}
	i = 0;
	while (i < params->cmd_count - 1)
	{
		if (pipe(params->pipes[i]) == -1)
		{
			// Free already allocated pipes
			perror("pipe");
			while (i >= 0)
				free(params->pipes[i--]);
			free(params->pipes);
			exit(1);
		}		// TODO: replace it with proper exiter
		++i;
	}
}

void	close_pipes(t_pipe *params)
{
	int	i;

	i = 0;
	while (i < params->cmd_count - 1)
	{
		close(params->pipes[i][0]);
		close(params->pipes[i][1]);
		++i;
	}
	return ;
}



// This function only used in child process
// This function exit with 1when error occurs.
void	apply_redirection(t_redir *r)
{
	while (r)
	{
		if (r->fd < 0)
		{
			// error handling
			perror(r->file);
			exit(1);
		}
		if (r->type == REDIR_IN || r->type == HEREDOC)
			dup2(r->fd, STDIN_FILENO);
		else if (r->type == APPEND || r->type == REDIR_OUT)
			dup2(r->fd, STDOUT_FILENO);
		close(r->fd);
		r = r->next;
	}
}
/** Check out run_child_process() */
void	child_process(t_shell *shell, t_cmd *cmd, t_pipe *param, int i)
{
	int	command_type;

	if (i > 0)
		dup2(param->pipes[i - 1][0], STDIN_FILENO);
	if (i < param->cmd_count - 1)
		dup2(param->pipes[i][1], STDOUT_FILENO);
	if (cmd->redir)
		apply_redirection(cmd->redir);

	close_pipes(param);
	free_pipes(param);
	
	// TODO: dealing with builtin functions
	if (cmd->cmd && cmd->cmd[0])
	{
		command_type = is_builtin(cmd->cmd[0]);
		if (command_type > 0)
		{
			execve_builtin(shell, command_type, cmd);
			exit(shell->exit);
		}
	}
	if (!cmd->path)
	{
		ft_putstr_fd(cmd->cmd[0], 2);
		ft_putstr_fd(" : command not found\n", 2);
		exit(127);
	}
	if (execve(cmd->path, cmd->cmd, env_to_array(shell->env_lst)) == -1)
	{
		perror(cmd->cmd[0]);
		exit(126);
	}
	// TODO: error handling after execve, 
}

/**checkout create_process() */
void	pipe_executor(t_shell *shell, t_pipe *params)
{
	t_cmd	*cmd;
	int	i;

	params->pids = ft_calloc(params->cmd_count, sizeof(pid_t));
	// TODO: error handling
	cmd = shell->cmd;
	i = 0;
	while (cmd)
	{
		params->pids[i] = fork();
		// TODO: success checka
		if (params->pids[i] == -1)
		{
			perror("fork");
			// 	params_cleaner(params);
			close_pipes(params);
			free_pipes(params);
			while (--i >= 0)
				kill(params->pids[i], SIGKILL);		// TODO: 这是啥？
			exit(EXIT_FAILURE);
		}
		if (params->pids[i] == 0)
			child_process(shell, cmd, params, i);
		cmd = cmd->next;
		++i;
	}
	close_pipes(params);
	free_pipes(params);
}

void	wait_handler(t_shell *shell, t_pipe *param)
{
	int	i;
	int	status;

	i = 0;
	while (i < param->cmd_count)
	{
		waitpid(param->pids[i], &status, 0);
		if (i == param->cmd_count - 1)
		{
			if (WIFEXITED(status))
				shell->prev_exit = WEXITSTATUS(status);		// TODO: need JD to confirm should be shell->exit. bc we have reset shell after execution
			else if (WIFSIGNALED(status))
				shell->prev_exit = 128 + WTERMSIG(status);
		}
		++i;
	}
	free(param->pids);
	param->pids = NULL;
}

// command such as "cd /tmp" needs to be executed in the parent process, 
// otherwise the environment variable doesn't change
void	executor2(t_shell *shell)
{
	t_pipe	params;
	int	command_type;

	command_type = 0;
	params = (t_pipe){0};
	params.cmd_count = count_cmd(shell->cmd);
	if (params.cmd_count == 1 && shell->cmd->cmd && shell->cmd->cmd[0])
	{
		command_type = is_builtin(shell->cmd->cmd[0]);
		if (command_type > 0)
		{
			if (shell->cmd->redir)
			{
				if (apply_redirection_parent(shell->cmd->redir) != 0)
				{
					shell->exit = 1;
					return ;
				}
			}
			execve_builtin(shell, command_type, shell->cmd);
			return ;
		}
	}
	if (params.cmd_count > 1)
		pipes_initializer(&params);
	else
		params.pipes = NULL;
	pipe_executor(shell, &params);
	wait_handler(shell, &params);
	// TODO: clean the params, t_pipe and ?t_shell?
}
