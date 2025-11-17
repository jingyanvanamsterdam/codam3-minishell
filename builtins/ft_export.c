#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static	t_env *
// TODO: 也许用JD做的那些个libft的添加env node的函数？？？

static void	add_or_update_env(t_env **env, char *key, char *value)
{
	t_env	*cur;

	cur = *env;
	while (cur)
	{
		if (!ft_strcmp(cur->key, key))
		{
			if (value)
			{
				free(cur->value);
				cur->value = ft_strdup(value);
			}
			return ;
		}
		cur = cur->next;
	}
	new = malloc(sizeof(t_env));

}

static void	add_or_update_env(t_env **env, char *key, char *value)
{
	t_env	*cur;
	t_env	*new;

	cur = *env;
	while (cur)
	{
		if (!ft_strcmp(cur->key, key))
		{
			if (value)
			{
				free(cur->value);
				cur->value = ft_strdup(value);
			}
			return ;
		}
		cur = cur->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	new->value = NULL;
	if (value)
		new->value = ft_strdup(value);
	new->next = *env;
	*env = new;
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
		return (ft_export_env_list(env), 0);
	while (argv[i])
	{
		sep = ft_strchr(argv[i], '=');
		if (sep)
		{
			*sep = '\0';
			add_or_update_env();
			*sep = '=';
		}
		else
			add_or_update_env();
		++i;
	}
	return (0);
}
