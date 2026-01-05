#include "struct.h"
#include "parse.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> // printf

static void	append_to_cmd_lst(t_cmd **head, t_cmd *node)
{
	t_cmd	*tmp;

	if (*head == NULL)
	{
		*head = node;
		return ;
	}
	tmp = *head;
	while(tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

/**
 * Function will remove quote symbol and accordingly handle expands if there is any.
 */
void	update_cmds_arr(char **cmd, t_token *token, t_shell *shell)
{
	size_t	i;

	i = 0;
	while (cmd[i])
		i++;
	cmd[i] = remove_quote(token->value, shell, false);
	if (!cmd[i])
		ft_malloc_failure("parsing.\n", shell);
}

void	update_cmd_redir(t_redir *redir, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = shell->cmd;
	while (cmd)
	{
		if (!cmd->next)
			cmd->redir = redir;
		cmd = cmd->next;
	}
}

/**
 * There is possibility that cmd len = 0 because only redirection info.
 * It is not input error or something need to be ignored to create node. 
 * cmd's path = NULL; change during execution process. 
 */
void	init_cmd_node(t_shell *shell, char **cmd)
{
	t_cmd	*node;

	node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!node)
	{
		free_2d_arr(cmd);
		ft_malloc_failure("parsing.\n", shell);
	}
	node->cmd = cmd;
	node->fd[0] = 0;
	node->fd[1] = 1;
	node->path = NULL;
	node->redir = NULL;
	node->next = NULL;
	append_to_cmd_lst(&(shell->cmd), node);
}

size_t	calculate_cmd_len(t_token *token)
{
	size_t	len;

	len = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == REDIR_IN || token->type == REDIR_OUT 
			|| token->type == HEREDOC || token->type == APPEND)
		{
			token = token->next;
			if (!token)
				return (len);
		}
		else
			len++;
		token = token->next;
	}
	return (len);
}