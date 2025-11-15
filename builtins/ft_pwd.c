#include "minishell.h"
#include "builtins.h"
#include "libft.h"

// return 0 if the function successes, return 1 otherwise
int	ft_pwd(char **argv, t_env *env)
{
	char	*cwd;
	
	(void)argv;
	(void)env;
	cwd = NULL;
	cwd = getcwd(cwd, 0);
	if (!cwd)
		return (1);
	ft_putstr_fd(cwd, 1);
	ft_putstr_fd("\n", 1);
	free(cwd);
	return (0);
}
