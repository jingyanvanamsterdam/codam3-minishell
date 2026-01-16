#include "minishell.h"
#include "libft.h"

char	*ft_getenv(t_env *env, const char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/*  This function creates a new env node, with a different inputs from create_node() */
t_env	*create_env_node_3args(char *key, char *value, t_shell *shell)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (ft_malloc_error("t_env allocation\n", shell), NULL);
	node->key = ft_strdup(key);
	if (!node->key)
		return (ft_malloc_error("t_env allocation\n", shell), NULL);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

void	update_env_value(t_shell *shell, char *key, char *value)
{
    t_env	*cur;

    cur = shell->env_lst;
    while (cur)
    {
        if (!ft_strcmp(cur->key, key))
        {
            free_charptr(&cur->value);
            if (value)
                cur->value = ft_strdup(value);
            return ;
        }
        cur = cur->next;
    }
    append_to_env_lst(&shell->env_lst, create_env_node_3args(key, value, shell));
}