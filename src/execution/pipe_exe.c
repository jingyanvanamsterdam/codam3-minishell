#include "minishell.h"
#include <fcntl.h>		// for pid_t
#include <stdlib.h>		// for EXIT_FAILURE
#include <sys/wait.h>
#include <unistd.h>		// for STDIN_FILENO
#include <stdio.h>		// for perror()
#include "libft.h"
#include <errno.h>

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
		return (ft_malloc_exe("pipes", shell, 0), 0);
	while (i < params->cmd_count - 1)
	{
		params->pipes[i] = malloc(sizeof(int) * 2);
		if (!params->pipes[i])
			return (ft_malloc_exe("pipe: ", shell, i), 0);
		if (pipe(params->pipes[i]) == -1)
			return (ft_pipe_error(shell, "pipe: ", i), 0);
		++i;
	}
	return (1);
}

int	create_process(t_shell *shell)
{
	t_pipe	*params;
	t_cmd	*cmd;
	int		i;
	//int		stream[2];

	params = shell->pip_param;
	params->pids = ft_calloc(params->cmd_count, sizeof(pid_t));
	if (!params->pids)
		return (ft_malloc_exe("pid", shell, params->cmd_count - 1), 0);
	cmd = shell->cmd;
	i = -1;
	while (++i < params->cmd_count)
	{
		params->pids[i] = fork();
		if (params->pids[i] < 0)
			return (ft_pipe_error(shell, "fork", params->cmd_count - 1), 0);
		else if (params->pids[i] == 0)
			run_child_process(shell, cmd, i);
		cmd = cmd->next;
	}
	close_pipes_i(shell->pip_param, params->cmd_count - 1);
	return (1);
}

void	run_child_process(t_shell *shell, t_cmd *cmd, int i)
{
	t_builtin	cmd_type;
	int			stream[2];

	sig_exe_child();
	if (cmd->cmd)
	{
		if (setup_stream(stream, cmd, i, shell))
		{
			cmd_type = is_builtin(cmd->cmd[0]);
			if (dup_files(shell, stream) == 0)
			{
				if (cmd_type != OTHERS)
					execve_builtin(shell, cmd_type, cmd);
				else
					execve_cmd(shell, cmd);
			}
		}
	}
	ft_process_exit(shell, false);
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
				shell->exit = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				shell->exit = 128 + WTERMSIG(status);
				write(1, "2\n", 2);
			}
		}
		++i;
	}
	free_pip_param(shell, shell->pip_param->cmd_count - 1);
}
