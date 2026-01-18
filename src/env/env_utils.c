/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 15:30:13 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 15:36:58 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "minishell.h"
#include "libft.h"
#include "env.h"

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

/**env->value will exist, when init_env, value == NULL, ft_strdup("") */
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
