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

/**return 0 if malloc fails. 
 * handle_redir_fd will return 0 only if heredoc has malloc error, which need to terminate the current paring.
 */
int	finish_set_tcmd(t_shell *shell)
{
	t_cmd	*cmd;
	char	**env_paths;

	cmd = shell->cmd;
	env_paths = create_env_path(shell->env_lst);
	if (!env_paths)
		return (ft_malloc_error("env path creation", shell), 0);
	while (cmd)
	{
		cmd->path = set_cmd_path(cmd->cmd[0], env_paths);
		if (!cmd->path)
			return (ft_malloc_error("path creation", shell), 0);
		if (cmd->redir)
			if (!handle_redir_fd(shell, cmd->redir))
				return (0);
		cmd = cmd->next;
	}
	return (1);
}

/**
 * This function parse tokens by types and update to the next token.
 * Check !shell->token;
 * if there is input error in redir, shell->token will be NULL
 */
t_token	*parse_token(t_token *token, t_shell *shell, char **cmd)
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
			if (!update_cmds_arr(cmd, token, shell))
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
	char	**cmd;
	size_t	size;

	token = shell->token;
	cmd = NULL;
	while (token)
	{
		size = calculate_cmd_len(token) + 1;
		cmd = ft_calloc(size, sizeof(char *));
		if (!cmd)
			return (ft_malloc_error("parsing.\n", shell), 0);
		if (!init_cmd_node(shell, cmd))
			return (free_2d_arr(cmd), 0);
		token = parse_token(token, shell, cmd);
		if (!shell->token)
			return (0);
	}
	if (!finish_set_tcmd(shell))
		return (0);
	return (1);
}
