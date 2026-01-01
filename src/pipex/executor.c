#include "struct.h"
#include <fcntl.h>		// for pid_t
#include <stdlib.h>		// for EXIT_FAILURE
#include <sys/wait.h>
#include <unistd.h>		// for STDIN_FILENO
#include "utils.h"

int	count_cmd(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

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
			return ;		// TODO: replace it with proper exiter
		++i;
	}
	i = 0;
	while (i < params->cmd_count - 1)
	{
		if (pipe(params->pipes[i]) == -1)
			return ;		// TODO: replace it with proper exiter
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

void	apply_rediction(t_redir *r)
{
	while (r)
	{
		if (r->type == REDIR_IN)
		{
			r->fd = open(r->file, O_RDONLY);
			dup2(r->fd, STDIN_FILENO);
			close(r->fd);
		}
		else if (r->type == REDIR_OUT)
		{
			r->fd = open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(r->fd, STDOUT_FILENO);
			close(r->fd);
		}
		else if (r->type == APPEND)
		{
			r->fd = open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(r->fd, STDOUT_FILENO);
			close(r->fd);
		}
		// HEREDOC handled earlier
		r = r->next;
	}
}

void	child_process(t_shell *shell, t_cmd *cmd, t_pipe *param, int i)
{
	if (i > 0)
		dup2(param->pipes[i - 1][0], STDIN_FILENO);
	if (i < param->cmd_count - 1)
		dup2(param->pipes[i][1], STDOUT_FILENO);
	if (cmd->redir)
		apply_redirection();

	close_pipes(param);
	free_pipes(param);

	// execve(path, argv, envp)
	execve(cmd->path, cmd->cmd, );		// TODO: Need a function that generate a envp variable

}

void	pipe_executor(t_shell *shell, t_pipe *params)
{
	t_cmd	*cmd;
	int	i;

	params->pids = ft_calloc(params->cmd_count, sizeof(pid_t));
	cmd = shell->cmd;
	i = 0;
	while (cmd)
	{
		params->pids[i] = fork();
		// TODO: success check
		// if (params->pids[i] == -1)
		// {
		// 	perror("fork");
		// 	params_cleaner(params);
		// 	exit(EXIT_FAILURE);
		// }
		if (params->pids[i] == 0)
			child_process();		// TODO: 
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
		if (i == param->cmd_count)
		{
			if (WIFEXITED(status))
				shell->prev_exit = WEXITSTATUS(status);		// TODO: need JD to confirm
		}
		++i;
	}
	free(param->pids);
	param->pids = NULL;
}

void	executor(t_shell *shell)
{
	int	status;
	t_pipe	params;

	params = (t_pipe){0};
	params.cmd_count = count_cmd(shell->cmd);	// or - 1?

	pipes_initializer(&params);

	// execute pipeline
	pipe_executor(shell, &params);

	wait_handler(shell, &params);

	// TODO: clean the params, t_pipe and ?t_shell?

}