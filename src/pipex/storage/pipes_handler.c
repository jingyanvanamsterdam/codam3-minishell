#include "struct.h"
#include "pipe.h"

void	pipes_initializer(t_pipe *params)
{
	int	i;

	params->pipes = malloc((params->count - 1) * sizeof(int *));
	if (!params->pipes)
	{
		perror("pipe array allocation");
		exit(1);
	}
	i = 0;
	while (i < params->count - 1)
	{
		params->pipes[i] = malloc(2 * sizeof(int));
		if (!params->pipes[i])
			pipes_initializer_exiter("pipe array allocation", i, params->pipes);		// TODO: replace it with an exiter for the whole program
		++i;
	}
	i = 0;
	while (i < params->count - 1)
	{
		if (pipe(params->pipes[i]) == -1)
			pipes_initializer_exiter("pipe", params->count - 1, params->pipes);
		++i;
	}
}

// TODO: 
// shell->cmd ======> ? =====> char **argv
// shell->env_lst ======> ? =====> char **envp

void	pipe_handler(t_shell *shell, t_pipe *params)
{
	int		i;
	char	**cmd;

	cmd = get_ith_cmd_from_list(shell->cmd, i);		// TODO: need to implement this function

	params->pids = ft_calloc(params->count, sizeof(pid_t));
	if (!params->pids)
	{
		perror("calloc in piping");
		params_cleaner(params);		// TODO: update the cleaner to clean the t_shell variable as well
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < params->count)
	{
		params->pids[i] = fork();
		if (params->pids[i] == -1)
		{
			perror("fork");
			params_cleaner(params);
			exit(EXIT_FAILURE);
		}
		if (params->pids[i] == 0)
			child_process(cmd, i, envp, params);
		++i;
	}
	close_pipes(params);
	free_pipes(params);
}

int	wait_handler(t_pipe *params)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	
	while (i < params->count)
	{
		waitpid(params->pids[i], &status, 0);
		if (i == params->count - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
		}
		++i;
	}
	free(params->pids);
	params->pids = NULL;
	return (last_status);
}
