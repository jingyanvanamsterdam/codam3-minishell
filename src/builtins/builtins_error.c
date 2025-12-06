#include "struct.h"
#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

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

// below are new functions and changed function :
void	free_redir_lst(t_redir **lst)
{
	t_redir	*temp;

	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->file)
			free((*lst)->file);
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

void	ft_malloc_failure(char	*s, t_shell *shell)
{
	ft_putstr_fd(s, 2);
	free_shell(shell);
	exit(EXIT_FAILURE);
}
