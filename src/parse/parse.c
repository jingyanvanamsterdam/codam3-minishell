#include "struct.h"
#include "parse.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> // printf

void	parsing(t_shell *shell)
{
	t_token *token;

	token = shell->token;
	// check first token is pipe or not. 
	if (token->type == PIPE)
		return (ft_input_error("minishell error: parse error near `|'", shell));
}
