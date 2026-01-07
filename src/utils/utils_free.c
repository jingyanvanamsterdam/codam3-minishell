#include <stdlib.h>
#include "parse.h" //change to minishell.h after combin
#include "utils.h"
#include <unistd.h> // close

// Changed name from free_split() 
void	free_2d_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		++i;
	}
	free(arr);
	arr = NULL;
}
/** shell->token = NULL after the function */
void	free_token_lst(t_token **lst)
{
	t_token	*temp;

	if (!lst || !(*lst))
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->value)
		{
			free((*lst)->value);
			(*lst)->value = NULL;
		}
		free(*lst);
		*lst = temp;
	}
}

/** only call when program need to get exit clean */
void	free_env_lst(t_env **lst)
{
	t_env	*temp;

	if (!lst || !(*lst))
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->key)
		{
			free((*lst)->key);
			(*lst)->key = NULL;
		}
		if ((*lst)->value)
		{
			free((*lst)->value);
			(*lst)->value = NULL;
		}
		free(*lst);
		*lst = temp;
	}
}

void	free_redir_lst(t_redir **lst)
{
	t_redir	*temp;

	if (!lst || !(*lst))
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->file)
		{
			free((*lst)->file);
			(*lst)->file = NULL;
		}
		if ((*lst)->fd != -1)
			close((*lst)->fd);
		free(*lst);
		*lst = temp;
	}
}

void	free_cmd_lst(t_cmd **lst)
{
	t_cmd	*temp;

	if (!lst || !(*lst))
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->cmd)
			free_2d_arr((*lst)->cmd);
		if ((*lst)->path)
		{
			free((*lst)->path);
			(*lst)->path = NULL;
		}
		if ((*lst)->redir)
			free_redir_lst(&((*lst)->redir));
		free(*lst);
		*lst = temp;
	}
}

/** need to check whether we need to free env_lst. only free when there is failutre. */
void	free_shell(t_shell *shell)
{
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->token)
		free_token_lst(&(shell->token));
	if (shell->env_lst)
		free_env_lst(&(shell->env_lst));
	if (shell->cmd)
		free_cmd_lst(&(shell->cmd));
	free_pipes(shell->pip_param);
	if (shell->pip_param->pids)
		free(shell->pip_param->pids);
	free(shell);
	shell = NULL;
}
// new function
void	free_quotok(t_quotok **lst)
{
	t_quotok	*tmp;

	if (!lst || !(*lst))
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->value)
		{
			free((*lst)->value);
			(*lst)->value = NULL;
		}
		free((*lst));
		*lst = tmp;
	}
}

void	ft_free_exit_process(int **pipes, t_shell *shell)
{
	int	n;

	n = count_cmd(shell->cmd) - 1;
	free_pipes_n(pipes, n);
	//exit process, to change, shouldn't be shell->exit.
	exit(shell->exit);
}

void	free_pipes_n(int **pipes, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(pipes[i]);
		pipes[i++] = NULL;
	}
	free(pipes);
	pipes = NULL;
	return ;
}

void	free_pipes(t_pipe *params)
{
	int	i;

	if (!params || !params->pipes)
		return ;
	i = 0;
	while (i < params->cmd_count - 1)
	{
		free(params->pipes[i]);
		params->pipes[i++] = NULL;
	}
	free(params->pipes);
	params->pipes = NULL;
	return ;
}
