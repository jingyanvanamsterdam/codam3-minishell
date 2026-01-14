#include <stdlib.h>
#include <stdio.h>
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

//void	free_pipes(t_pipe *params)
//{
//	int	i;

//	if (!params || !params->pipes)
//		return ;
//	i = 0;
//	while (i < params->cmd_count - 1)
//	{
//		free(params->pipes[i]);
//		params->pipes[i++] = NULL;
//	}
//	free(params->pipes);
//	params->pipes = NULL;
//	return ;
//}

/** if the char ptr exist, free and set as NULL */
void	free_charptr(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}