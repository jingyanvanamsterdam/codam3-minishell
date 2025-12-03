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
char	*handle_dquote(char *str, size_t len, t_shell *shell)
{
	char	*res;
	size_t	exp_i;

	res = NULL;
	exp_i = find_index(str, len, '$');
	if (exp_i < len)
		res = handle_expands(str, len - 1, shell);
	else
		res = ft_substr(str, 0, len - 1);
	if (!res)
		return (NULL);
	return (res);
}

char	*handle_token(t_type t, t_token *token, t_shell *shell)
{
	size_t	quote_i;
	size_t	expand_i;
	size_t	len;
	char	*value;
	int 	i;

	len = ft_strlen(token->value);
	while (i < len)
	{
		quote_i = quote_index(token->value + i, len - i);
		expand_i = find_index(token->value + i, len - i, '$');
		if (t == HEREDOC)
			value = ft_strdup(token->value);
		else if (expand_i < quote_i)
			value = handle_expands(token->value + i, quote_i - expand_i, shell);
		else
			value = ft_substr(token->value + i, 0, quote_i); //if no expands before quote, then deal with anything before quote.
		if (!value)
			ft_malloc_failure("parsing.\n", shell);
		if (t != HEREDOC && token->value[quote_i] == '\'')
			value = append_to_str(value, ft_substr(token->value + i, quote_i + 1, len - i - quote_i - 2));
		else if (t != HEREDOC && quote_i < len)
			value = append_to_str(value, handle_dquote(token->value + i + quote_i + 1, len - i - quote_i - 1, shell));
		if (!value)
			ft_malloc_failure("parsing.\n", shell);
		//i += something.
	}
	return (value);
}

/**
 * update the token to next one, if it is NULL or != WORD, means after redir symbol, there is an input error.
 * ft_input_error() will clean up shell's cmd, token and env_lst. 
 * so if shell->token == NULL, means there is input error and cleaned up shell and enter next readline loop.
 */
t_token	*handle_redir(t_token *token, t_redir **redir, t_shell *shell)
{
	t_redir *node;
	char	*value;
	t_type	t;

	value = token->value;
	t = token->type;
	token = token->next;
	if (!token || token->type != WORD)
		return (ft_input_error("near upexpected token `", value, shell), NULL);
	node = (t_redir *)malloc(sizeof(t_redir));
	if (!node)
		ft_malloc_failure("parsing.\n", shell);
	node->type = t;
	node->file = handle_token(t, token, shell);
	if (!node->file)
		ft_malloc_failure("parsing.\n", shell);
	node->next = NULL;
	append_to_rdir_lst(redir, node);
	return (token);
}
