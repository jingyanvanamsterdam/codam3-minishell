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

/** heredoc has malloc. so need to return 0 if heredoc returned -1
 * at this step, fd might opened, so exit need to close fds
 */
int	handle_redir_fd(t_shell *shell, t_redir *redir)
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
			return (close_cmd_fds(shell), 0);
		else if (redir->fd == -1)
			ft_error_printing(redir->file);
		redir = redir->next;
	}
	return (1);
}

static char	*find_infile(t_redir *redir, t_cmd *cmd)
{
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			cmd->intype = redir->type;
			return (redir->file);
		}
		redir = redir->next;
	}
	return (NULL);
}

static char	*find_outfile(t_redir *redir, t_cmd *cmd)
{
	while (redir)
	{
		if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			cmd->intype = redir->type;
			return (redir->file);
		}
		redir = redir->next;
	}
	return (NULL);
}

int	handle_cmd_io(t_shell *shell, t_cmd *cmd)
{
	t_redir	*redir;
	char	*infile;
	char	*outfile;

	redir = cmd->redir;
	infile = find_infile(redir, cmd);
	outfile = find_outfile(redir, cmd);
	if (cmd->intype != DEFAULT)
	{
		cmd->infile = ft_strdup(infile);
		if (!cmd->infile)
			return (ft_malloc_error("cmd io", shell), 0);
	}
	if (cmd->outtype != DEFAULT)
	{
		cmd->outfile = ft_strdup(outfile);
		if (!cmd->outfile)
			return (ft_malloc_error("cmd io", shell), 0);
	}
	return (1);
}

/**
 * Return value = next token, if it is NULL or != WORD, means after redir symbol, there is an input error.
 * Check !shell->token; ft_malloc_parsing clean up token list and cmd list
 * ft_input_error() will clean up shell's cmd, token. 
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
		return (ft_malloc_parsing("parsing", shell), NULL);
	node->type = t;
	if (t == HEREDOC)
		node->file = ft_strdup(token->value);
	else
		node->file = remove_quote(token->value, shell, false);
	if (!node->file)
		return (ft_malloc_parsing("parsing", shell), NULL);
	node->fd = -1;
	node->next = NULL;
	append_to_rdir_lst(redir, node);
	return (token);
}
