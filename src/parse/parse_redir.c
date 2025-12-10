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

void	handle_redir_fd(t_shell *shell, t_redir *redir)
{
	while (redir)
	{
		if (redir->type == REDIR_IN)
			redir->fd = open_infile(redir->file);
		else if (redir->type == REDIR_OUT)
			redir->fd = open_outfile(redir->file);
		else if (redir->type == APPEND)
			redir->fd = output_append(redir->file);
		else if (redir->type == HEREDOC)
			redir->fd = heredoc(shell, redir);
		if (redir->fd == -1 && redir->type == HEREDOC)
			ft_error_printing("open heredoc");
		else if (redir->fd == -1)
			ft_error_printing(redir->file);
		redir = redir->next;
	}
}

/**
 * Return value = next token, if it is NULL or != WORD, means after redir symbol, there is an input error.
 * ft_input_error() will clean up shell's cmd, token and env_lst. 
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
	if (t == HEREDOC)
		node->file = ft_strdup(token->value);
	else
		node->file = remove_quote(token->value, shell, false);
	if (!node->file)
		ft_malloc_failure("parsing.\n", shell);
	node->fd = -1;
	node->next = NULL;
	append_to_rdir_lst(redir, node);
	return (token);
}
