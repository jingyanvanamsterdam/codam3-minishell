#include <stdlib.h>
#include <stdio.h>
#include "parse.h" //change to minishell.h after combin
#include "utils.h"
#include <unistd.h> // close

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
        free_charptr(&((*lst)->path));
		if ((*lst)->redir)
			free_redir_lst(&((*lst)->redir));
		free(*lst);
		*lst = temp;
	}
}

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
