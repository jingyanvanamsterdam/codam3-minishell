#include "struct.h"
#include "parse.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> // printf

t_token	*parsing_before_pipe(t_token *token, t_shell *shell, char **cmd)
{
	t_redir *redir;

	redir = NULL; 
	if (token->type == PIPE)
		return (ft_input_error(token->value, shell), NULL);
	create_cmd_node(shell, cmd);
	while (token && token->type != PIPE)
	{
		if (token->type == REDIR_IN || token->type == REDIR_OUT 
			|| token->type == HEREDOC || token->type == APPEND)
		{
			token = handle_redir(token->type, token, &redir, shell);
			if (!shell->token)
				return (NULL);
		}
		else
			append_to_cmd(cmd, token, shell);
		token = token->next;
	}
	shell->cmd->redir = redir;
	if (token && token->type == PIPE)
		return (token->next);
	return (token);
}

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
			ft_malloc_failure("Malloc failed at parsing.\n", shell);
		token = parsing_before_pipe(token, shell, cmd);
		if (!shell->token)
			return (free_2d_arr(cmd));
	}
}
