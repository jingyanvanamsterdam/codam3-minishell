#include "struct.h"
#include "parse.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> // printf

static void	append_to_rdir_lst(t_redir **head, t_redir *node)
{
	t_redir	*tmp;

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

void	append_to_cmd(char **cmd, char *src, t_shell *shell)
{
	size_t	i;

	i = 0;
	while (cmd[i])
		i++;
	cmd[i] = ft_strdup(src);
	if (!cmd[i])
	{
		free_2d_arr(cmd);
		ft_malloc_failure("Malloc failed at parsing.\n", shell);
	}
}
/**
 * update the token to next one, if it is NULL or != WORD, there is input error.
 */
t_token	*handle_redir(t_type t, t_token *token, t_redir **redir, t_shell *shell)
{
	t_redir *node;
	char	*value;

	value = token->value;
	token = token->next;
	if (!token || token->type != WORD)
		return (ft_input_error(value, shell), NULL);
	node = (t_redir *)malloc(sizeof(t_redir));
	if (!node)
		ft_malloc_failure("Malloc failed at parsing.\n", shell);
	node->type = t;
	node->file = ft_strdup(token->value);
	if (!(node->file))
		ft_malloc_failure("Malloc failed at parsing.\n", shell);
	node->next = NULL;
	append_to_rdir_lst(redir, node);
	return (token);
}

/**
 * There is possibility that cmd len = 0 because only redirection info.
 * It is not input error or something need to be ignored to create node. 
 * cmd's path = NULL; change during execution process. 
 */
void	create_cmd_node(t_redir *redir, t_shell *shell, char **cmd)
{
	t_cmd	*node;

	node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!node)
		ft_malloc_failure("Malloc failed at parsing.\n", shell);
	node->cmd = cmd;
	node->path = NULL;
	node->redir = redir;
	node->next = NULL;
	append_to_cmd_lst(&(shell->cmd), node);
}
