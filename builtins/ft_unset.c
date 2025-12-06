
#include "libft.h"
#include "minishell.h"

// valid env variable is: [A-Za-z_][A-Za-z0-9_]*
int	is_valid_env_key(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		++i;
	}
	return (1);
}

void	remove_env_key(t_env **env, char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (!ft_strcmp(cur->key, key))
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return;
		}
		prev = cur;
		cur = cur->next;
	}
}

int	ft_unset(char **argv, t_shell *shell)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if(is_valid_env_key(argv[i]))
			remove_env_key(&(shell->env_lst), argv[i]);
		++i;
	}
	return (0);
}
