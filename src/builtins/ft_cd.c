
#include <unistd.h>
#include <stdio.h>
#include "libft.h"
#include "minishell.h"

static t_env	*create_node3(const char *key, const char *value, t_shell *shell)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	// should use the ft_malloc_failure() implementation in JD's branch
	(void)shell;
	// if (!node)
	// 	ft_malloc_failure("Failure at malloc env.\n", shell);
	node->key = ft_strdup(key);
	// if (!node->key)
	// 	ft_malloc_failure("Failure at malloc env.\n", shell);
	if (value)
		node->value = ft_strdup(value);
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

void	update_env_value(t_shell *shell, const char *key, char *value)
{
	t_env	*cur;

	cur = shell->env_lst;
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
	append_to_env_lst(&shell->env_lst, create_node3(key, value, shell));
}

void	print_cd_error(const char *path)
{
	ft_putstr_fd("cd: ", 2);
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
	else if (!ft_strcmp(argv[1], "-"))
	{
		target = ft_getenv(shell->env_lst, "OLDPWD");
		if (!target)
			return (ft_putstr_fd("cd: OLDPWD not set\n", 2), 1);
		// ft_putendl_fd(target, 1);
		ft_putstr_fd(target, 1);
		ft_putstr_fd("\n", 1);
	}
	else
		target = argv[1];

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		oldpwd = ft_strdup("");

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
