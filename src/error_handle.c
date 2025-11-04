#include "struct.h"
#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

void	ft_malloc_failure(char	*s, t_env *env_list)
{
	ft_putstr_fd(s, 2);
	if (env_list)
		free_env_list(env_list);
	exit(EXIT_FAILURE);
}