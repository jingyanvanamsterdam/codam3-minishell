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
	node->file = handle_token(t, token, shell);
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

char	*handle_token(t_type t, t_token *token, t_shell *shell)
{
	size_t	quote_i;
	size_t	expand_i;
	size_t	len;
	char	*value;

	quote_i = quote_index(token->value, len);
	expand_i = find_index(token->value, len, '$');
	if (expand_i < quote_i && t != HEREDOC)
		value = handle_expands(token->value, quote_i - expand_i, shell);
	else
		value = ft_substr(token->value, 0, quote_i); //if no expands before quote, then deal with anything before quote.
	if (!value)
		ft_malloc_failure_parsing("Malloc failed at parsing\n", shell);	
	if (t == HEREDOC || token->value[quote_i] == '\'')
		value = append_to_str(value, ft_substr(token->value, quote_i + 1, len - quote_i - 2));
	else if (quote_i < len)
		value = append_to_str(value, handle_quote(token->value + quote_i + 1, len - quote_i, shell));
	if (!value)
		ft_malloc_failure_parsing("Malloc failed at parsing\n", shell);
	return (value);
}

char	*handle_quote(char *str, size_t len, t_shell *shell)
{
	char	*res;
	size_t	exp_i;

	res = NULL;
	exp_i = find_index(str, len, '$');
	if (exp_i < len)
		res = handle_expands(str, len - 1, shell);
	if (!res)
		return (NULL);
	return (res);
}
