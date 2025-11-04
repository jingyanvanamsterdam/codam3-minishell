#include "struct.h"
#include "lex.h"
#include "libft.h"
#include <stdlib.h> 

void	ft_token_failure(char *s, t_token *token)
{
	ft_putstr_fd(s, 2);
	if (token)
		free_token_list(token);
	exit(EXIT_FAILURE);
}

// if we do exit at malloc, then need to free t_shell. To change later.