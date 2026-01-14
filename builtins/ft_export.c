#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static t_env	*create_node(char **key_value, t_shell *shell)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
	{
		free_2d_arr(key_value);
		ft_malloc_failure("Failure at malloc env.\n", shell);
	}
	node->key = ft_strdup(key_value[0]);
	if (!node->key)
	{
		free_2d_arr(key_value);
		ft_malloc_failure("Failure at malloc env.\n", shell);
	}
	if (key_value[1])
		node->value = ft_strdup(key_value[1]);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}
// TODO: 也许用JD做的那些个libft的添加env node的函数？？？

static void	append_to_env_lst(t_env **head, t_env *node)
{
	t_env	*cur;

	if (*head == NULL)
	{
		*head = node;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

static void	add_or_update_env(t_env **env, char *key, char *value, t_shell *shell)
{
	t_env	*cur;
	char	*kv[3];

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
	kv[0] = key;
	if (value)
		kv[1] = value;
	else
		kv[1] = "";
	kv[2] = NULL;
	append_to_env_lst(env, create_node(kv, shell));
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
int	ft_export(char **argv, t_shell *shell)
{
	int	i;
	char	*sep;

	i = 1;
	if (!argv[i])
		return (ft_export_env_list(shell->env_lst), 0);
	while (argv[i])
	{
		sep = ft_strchr(argv[i], '=');
		if (sep)
		{
			// add new env variableq
			*sep = '\0';
			add_or_update_env(&(shell->env_lst), argv[i], sep + 1, shell);
			*sep = '=';
		}
		else
			add_or_update_env(&(shell->env_lst), argv[i], NULL, shell);
		++i;
	}
	return (0);
}
