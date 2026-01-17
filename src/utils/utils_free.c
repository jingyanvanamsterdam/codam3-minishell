#include <stdlib.h>
#include <stdio.h>
#include "parse.h" //change to minishell.h after combin
#include "utils.h"
#include <unistd.h> // close

// Changed name from free_2d_arr() 
void	free_2d_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free_charptr(&arr[i]);
		++i;
	}
	free(arr);
	arr = NULL;
}

/** Only call free_shell when the minishell is exiting*/
void	free_shell(t_shell *shell)
{
	if (shell->input)
		free_charptr(&(shell->input));
	if (shell->env_lst)
		free_env_lst(&(shell->env_lst));
	if (shell->token)
		free_token_lst(&(shell->token));
	if (shell->cmd)
		free_cmd_lst(&(shell->cmd));
	if (shell->pip_param)
		free_pip_param(shell, shell->pip_param->cmd_count - 1);
	free(shell);
	shell = NULL;
}

void	free_pip_param(t_shell *shell, int	n)
{
	if (shell->pip_param)
	{
		if (shell->pip_param->pipes)
			free_pipes_n(shell->pip_param, n);
		if (shell->pip_param->pids)
		{
			free(shell->pip_param->pids);
			shell->pip_param->pids = NULL;
		}
		free(shell->pip_param);
		shell->pip_param = NULL;
	}
}

void	free_pipes_n(t_pipe *params, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(params->pipes[i]);
		params->pipes[i++] = NULL;
	}
	free(params->pipes);
	params->pipes = NULL;
	return ;
}


/** if the char ptr exist, free and set as NULL */
void	free_charptr(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}