// t_env *env_find(t_env *env, const char *key);
// char *env_get_value(t_env *env, const char *key);
// char **env_to_array(t_env *env);
// int env_key_exists(t_env *env, const char *key);

#include "struct.h"
#include "minishell.h"
#include "libft.h"
#include "env.h"

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

// count the number of nodes in env
static int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		++count;
		env = env->next;
	}	
	return (count);
}

static void	free_array(char **arr, int n)
{
	while (n > 0)
	{
		free(arr[n - 1]);
		--n;
	}
	free(arr);
}

static char	*join_key_value(t_env *env)
{
	char	*pair;
	size_t	len;

	len = ft_strlen(env->key) + 2;
	if (env->value)
		len += ft_strlen(env->value);
	pair = malloc(len);
	if (!pair)
		return (NULL);
	ft_strlcpy(pair, env->key, len);
	ft_strlcat(pair, "=", len);
	if (env->value)
		ft_strlcat(pair, env->value, len);
	return (pair);
}

// convert env list to a "key=value" string array for execve()
char	**env_to_array(t_env *env)
{
	char	**arr;
	int		i;

	arr = malloc(sizeof(char *) * (env_count(env) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i] = join_key_value(env);
		if (!arr[i])
		{
			free_array(arr, i);
			return (NULL);
		}
		env = env->next;
		++i;
	}
	arr[i] = NULL;
	return (arr);
}
