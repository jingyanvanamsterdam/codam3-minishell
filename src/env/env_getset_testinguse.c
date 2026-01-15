#include "minishell.h"
#include "libft.h"
#include "env.h"

// if the key exist, update its value
// if not, append a new node at the end
void	env_set(t_env **env, const char *key, const char *value)
{
	t_env	*tmp;
	t_env	*new;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	new = malloc(sizeof(t_env));
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	tmp->next = new;
}

// remove the node, followed by adjusting pointers and memory free
void	env_unset(t_env **env, const char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

// find the node matching the given key
t_env	*env_find(t_env *env, const char *key)
{
    if (!env || !key)
        return (NULL);
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (env);
        env = env->next;
    }
    return (NULL);
}

// return only the value of the given key (NULL if not found)
char    *env_get_value(t_env *env, const char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

int	env_key_exists(t_env *env, const char *key)
{
	return (env_find(env, key) != NULL);
}