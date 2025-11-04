#include <stdlib.h>
#include "minishell.h"

void	free_split(char **key_value)
{
	int	i;

	if (!key_value)
		return ;
	i = 0;
	while (key_value[i])
	{
		free(key_value[i]);
		key_value[i] = NULL;
		++i;
	}
	free(key_value);
	key_value = NULL;
}

void	free_env_list(t_env *env_list)
{
	t_env	*tmp;
	while (env_list)
	{
		if (env_list->key)
			free(env_list->key);
		if (env_list->value)
			free(env_list->value);
		tmp = env_list->next;
		free(env_list);
		env_list = tmp;
	}
}