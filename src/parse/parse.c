#include "struct.h"
#include "parse.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> // printf


//void	print_quotok(t_quotok *head)
//{
//	while (head)
//	{
//		printf("printing quotok %s\n", head->value);
//		head = head->next;
//	}
//	printf("==========finish printing==========\n");
//}

int	finish_set_tcmd(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = shell->cmd;
	while (cmd)
	{
		if (!handle_cmd_heredoc(cmd, shell))
			return (0);
		cmd = cmd->next;
	}
	if (!set_tcmd_pathes(shell))
		return (0);
	return (1);
}

/**
 * This function parse tokens by types and update to the next token.
 * Check !shell->token;
 * if there is input error in redir, shell->token will be NULL
 */
t_token	*parse_token(t_token *token, t_shell *shell, t_cmd *cmd)
{
	t_redir *redir;

	redir = NULL; 
	if (token->type == PIPE)
		return (ft_input_error("near upexpected token `", token->value, shell), NULL);
	while (token && token->type != PIPE)
	{
		if (token->type == REDIR_IN || token->type == REDIR_OUT 
			|| token->type == HEREDOC || token->type == APPEND)
		{
			token = handle_redir(token, &redir, shell);
			if (!shell->token)
				return (NULL);
		}
		else
			if (!update_cmds_arr(cmd->cmd, token, shell))
				return (NULL);
		token = token->next;
	}
	update_cmd_redir(redir, shell);
	if (token && token->type == PIPE)
		return (token->next);
	return (token);
}

/**
 * If there is input error, check shell->token == NULL
 * return 0, if there is malloc fail
 */
int	parsing(t_shell *shell)
{
	t_token	*token;
	size_t	size;
	t_cmd	*cmd;
	t_cmd	*tmp;

	token = shell->token;
	while (token)
	{
		size = calculate_cmd_len(token) + 1;
		if (!init_cmd_node(shell, size))
			return (0);
		tmp = shell->cmd;
		while (tmp)
		{
			cmd = tmp;
			tmp = tmp->next;
		}
		token = parse_token(token, shell, cmd);
		if (!shell->token)
			return (0);
	}
	if (!finish_set_tcmd(shell))
		return (0);
	return (1);
}
