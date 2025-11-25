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
/**
 * Function will remove the quote from cmds. 
 */
void	append_to_cmd(char **cmd, t_token *token, t_shell *shell)
{
	size_t	i;

	i = 0;
	while (cmd[i])
		i++;
	cmd[i] = handle_token(WORD, token, shell);
	if (!cmd[i])
		ft_malloc_failure("parsing.\n", shell);
}


/**
 * There is possibility that cmd len = 0 because only redirection info.
 * It is not input error or something need to be ignored to create node. 
 * cmd's path = NULL; change during execution process. 
 */
void	create_cmd_node(t_shell *shell, char **cmd)
{
	t_cmd	*node;

	node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!node)
	{
		free_2d_arr(cmd);
		ft_malloc_failure("parsing.\n", shell);
	}
	node->cmd = cmd;
	node->path = NULL;
	node->redir = NULL;
	node->next = NULL;
	append_to_cmd_lst(&(shell->cmd), node);
}
