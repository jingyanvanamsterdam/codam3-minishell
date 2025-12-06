
#include "libft.h"
#include "minishell.h"

// need functions like: ft_getenv() & update_env_value()
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

void	update_env_value(t_shell *shell, const char *key, const char *value)
{
	t_env	*cur;

	while (cur)
	{
		if (!ft_strcmp(cur->key, key))
		{
			free(cur->value);
			cur->value = ft_strdup(value);
			return ;
		}
		cur = cur->next;
	}
	add_env_node(&shell->env_lst, key, value);
}

void	print_cd_error(const char *path)
{
	ft_pustr_fd("cd: ", 2);
	perror(path);
}

int	ft_cd(char **argv, t_shell *shell)
{
	char	*target;
	char	*oldpwd;
	char	*newpwd;

	if (!argv[1])	// cd with no args -> go to HOME
	{
		target = ft_getenv(shell->env_lst, "HOME");
		if (!target)
			return (ft_putstr_fd("cd: HOME not set\n", 2), 1);
	}
		target = argv[1];
	oldpwd = getcwd(NULL, 0);
	if (chdir(target) == -1)
	{
		free(oldpwd);
		return (print_cd_error(target), 1);
	}
	update_env_value(shell, "OLDPWD", oldpwd);
	free(oldpwd);
	newpwd = getcwd(NULL, 0);
	update_env_value(shell, "PWD", newpwd);
	free(newpwd);
	return (0);
}
