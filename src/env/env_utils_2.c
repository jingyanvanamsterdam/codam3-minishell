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
			{
                cur->value = ft_strdup(value);
				if (!cur->value)
					ft_malloc_error("env value update", shell);
			}
            return ;
        }
        cur = cur->next;
    }
	cur = create_env_node_3args(key, value, shell);
	if (!cur)
		return (ft_malloc_error("env node creation", shell));
    append_to_env_lst(&shell->env_lst, cur);
}

t_env	*create_node(char **key_value, t_shell *shell)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env) * 1);
	if (!node)
		return (ft_malloc_error("env setup", shell), NULL);
	node->key = ft_strdup(key_value[0]);
	if (key_value[1])
		node->value = ft_strdup(key_value[1]);
	else
		node->value = ft_strdup("");
	if (!node->key || !node->value)
		return (ft_malloc_error("env setup", shell), NULL);
	node->next = NULL;
	return (node);
}

void	append_to_env_lst(t_env **head, t_env *node)
{
	t_env	*tmp;

	//if (!head || !node)
	//	return ;
	if (*head == NULL)
	{
		*head = node;
		return ;
	}
	tmp = *head;
	while(tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}
