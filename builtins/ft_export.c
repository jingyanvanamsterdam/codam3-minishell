#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void	add_or_update_env()
{

}

static void	ft_export_env_list(t_env *env)
{
	while (env)
	{
		printf("declare -x %s", env->key);
		if (env->value)
			printf("=\"%s\"", env->value);
		printf("\n");
		env = env->next;
	}
}

// "export [key=value] [key=value]"
int	ft_export(char **argv, t_env *env)
{
	int	i;
	char	*sep;
	t_env	*existing;

	(void)env;
	i = 1;
	if (!argv[i])
		return (ft_export_env_list(env), 0);			// TODO: replace the following section with a function
	while (argv[i])
	{
		sep = ft_strchr(argv[i], '=');
		if (sep)
		{
			// add new env variableq
			*sep = '\0';

			*sep = '=';
		}
		else
		++i;
	}
	return (0);
}
