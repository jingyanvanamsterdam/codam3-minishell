#include "libft.h"
#include "minishell.h"
#include <unistd.h>

// TODO: 这个函数和env里面的打印整个env list的一样呢，到时候可以把env里那个函数给替换掉
int	ft_env(char **argv, t_shell *shell)
{
	t_env	*env;

	(void)argv;
	env = shell->env_lst;
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd(env->key, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\n", 1);
		}
		env = env->next;
	}
	return (0);
}
