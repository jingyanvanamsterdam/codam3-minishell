#include <stdlib.h>
#include "parse.h" //change to minishell.h after combin
#include "pipe.h"
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

void	free_token_lst(t_token **lst)
{
	t_token	*temp;

	if (!lst)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->value)
			free((*lst)->value);
		free(*lst);
		*lst = temp;
	}
}

void	free_env_lst(t_env **lst)
{
	t_env	*temp;

	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->key)
			free((*lst)->key);
		if ((*lst)->value)
			free((*lst)->value);
		free(*lst);
		*lst = temp;
	}
}

void	free_redir_lst(t_redir **lst)
{
	t_redir	*temp;

	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->file)
			free((*lst)->file);
		if ((*lst)->fd != -1)
			close((*lst)->fd);
		free(*lst);
		*lst = temp;
	}
}

void	free_cmd_lst(t_cmd **lst)
{
	t_cmd	*temp;

	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->cmd)
			free_2d_arr((*lst)->cmd);
		if ((*lst)->path)
			free((*lst)->path);
		if ((*lst)->redir)
			free_redir_lst(&((*lst)->redir));
		free(*lst);
		*lst = temp;
	}
}

void	free_shell(t_shell *shell)
{
	if (shell->token)
		free_token_lst(&(shell->token));
	if (shell->env_lst)
		free_env_lst(&(shell->env_lst));
	if (shell->cmd)
		free_cmd_lst(&(shell->cmd));
	free(shell);
}
// new function
void	free_quotok(t_quotok **lst)
{
	t_quotok	*tmp;

	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->value)
			free((*lst)->value);
		free((*lst));
		*lst = tmp;
	}
}

void	free_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(pipes[i]);
		pipes[i] = NULL;
		i++;
	}
	free(pipes);
	pipes = NULL;
}

void	ft_free_exit_process(int **pipes, t_shell *shell)
{
	int	n;

	n = count_cmd(shell->cmd) - 1;
	free_pipes(pipes, n);
	//exit process?
	exit(shell->exit);
}

