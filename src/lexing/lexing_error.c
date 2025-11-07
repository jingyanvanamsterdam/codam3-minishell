#include "struct.h"
#include "lex.h" //change to minishell.h after combin
#include "libft.h"
#include <stdlib.h> 

void	ft_malloc_failure(char *s, t_shell *shell)
{
	ft_putstr_fd(s, 2);
	if (shell->token)
		free_token_list(&(shell->token));
	if (shell->env_lst)
		free_env_list(&(shell->env_lst));
	free(shell);
	exit(EXIT_FAILURE);
}
