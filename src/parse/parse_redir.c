#include "struct.h"
#include "parse.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> // printf
#include <stdbool.h>



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
