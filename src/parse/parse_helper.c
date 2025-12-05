#include "struct.h"
#include "parse.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> // printf
#include <stdbool.h>

void	print_quotok(t_quotok *head)
{
	while (head)
	{
		printf("printing quotok %s\n", head->value);
		head = head->next;
	}
	printf("==========finish printing==========\n");
}

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

char	*join_quotok(t_quotok *quotok)
{
	char	*value;
	char	*tmp;

	value = NULL;
	tmp = NULL;
	//printf("quotok is created\n");
	//print_quotok(quotok);
	value = ft_strdup(quotok->value);
	if (!value)
		return (NULL);
	quotok = quotok->next;
	while (quotok)
	{
		//printf("find next quotok: value = %s\n", quotok->value);
		tmp = value;
		value = ft_strjoin(tmp, quotok->value);
		free(tmp);
		if (!value)
			return (NULL);
		quotok = quotok->next;
	}
	//printf("Joined tokens, value = %s\n", value);
	return (value);
}



char	*remove_quote(char *value, t_shell *shell, bool hdoc)
{
	t_quotok *quotok;
	char	*res;

	quotok = NULL;
	res = NULL;
	quotok = tokenize_quote(value, shell, hdoc);
	//print_quotok(quotok);
	res = join_quotok(quotok);
	free_quotok(&quotok);
	if (!res)
		return (NULL);
	//printf("res=%s\n", res);
	return (res);
}

char	*handle_token(t_type t, t_token *token, t_shell *shell)
{
	char	*value;
	

	value = NULL;
	if (t == HEREDOC)
		value = ft_strdup(token->value);
	else
		value = remove_quote(token->value, shell, false);
	if (!value)
		ft_malloc_failure("parsing\n", shell);
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
