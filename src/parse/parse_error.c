#include "struct.h"
#include "parse.h" //change to minishell.h after combin
#include "libft.h"
#include <stdlib.h> 

void	ft_malloc_failure(char *s, t_shell *shell)
{
	ft_putstr_fd(s, 2);
	free_shell(shell);
	exit(EXIT_FAILURE);
}

void	ft_input_error(char	*s, t_shell *shell)
{
	ft_putstr_fd(s, 2);
	if (shell->token)
		free_token_lst(&(shell->token));
	shell->token = NULL;
	if (shell->cmd)
		free_cmd_lst((&shell->cmd));
	shell->cmd = NULL;
}

// new funcs are below: