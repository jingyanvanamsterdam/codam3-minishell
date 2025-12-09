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

/**
 * This function parse tokens by types and update to the next token.
 * if there is input error in redir, shell->token will be NULL, the shell will cleaned up and return NULL.
 *  
 */
t_token	*parse_token(t_token *token, t_shell *shell, char **cmd)
{
	t_redir *redir;

	redir = NULL; 
	if (token->type == PIPE)
	{
		ft_input_error("near upexpected token `", token->value, shell);
		return (NULL);
	}
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
			update_cmds_arr(cmd, token, shell);
		token = token->next;
	}
	update_cmd_redir(redir, shell);
	if (token && token->type == PIPE)
		return (token->next);
	return (token);
}

/**
 * If there is input error, check shell->token == NULL
 */
void	parsing(t_shell *shell)
{
	t_token	*token;
	char	**cmd;
	size_t	size;

	token = shell->token;
	cmd = NULL;
	while (token)
	{
		size = calculate_cmd_len(token) + 1;
		cmd = ft_calloc(size, sizeof(char *));
		if (!cmd)
			ft_malloc_failure("parsing.\n", shell);
		init_cmd_node(shell, cmd);
		token = parse_token(token, shell, cmd);
		if (!shell->token)
			return ;
	}
}
