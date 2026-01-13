#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// TODO: 
/* Need to handle cases:
	Should exit with 1:
		export HELLO-=123
		export = 
		export 123
		export A-
 */

static int	is_valid_identifier(const char *identifier)
{
	int	i;

	if (!identifier || !identifier[0])
		return (0);
	// First character must be a letter or underscore
	if (!ft_isalpha(identifier[0]) && identifier[0] != '_')
		return (0);
	// Remaining characters must be alphanumeric or underscore
	i = 1;
	while (identifier[i])
	{
		if (!ft_isalnum(identifier[i]) && identifier[i] != '_')
			return (0);
		++i;
	}
	return (1);
}

static void	print_export_error(char *identifier)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(identifier, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

// static t_env	*create_node2(char **key_value, t_shell *shell)
// {
// 	t_env	*node;

// 	node = (t_env *)malloc(sizeof(t_env));
// 	if (!node)
// 		return (free_2d_arr(key_value), ft_malloc_error("ft_export t_env allocation\n", shell), NULL);
// 	node->key = ft_strdup(key_value[0]);
// 	if (!node->key)
// 		return (free_2d_arr(key_value), ft_malloc_error("ft_export t_env allocation\n", shell), NULL);
// 	if (key_value[1])
// 		node->value = ft_strdup(key_value[1]);
// 	else
// 		node->value = NULL;
// 	node->next = NULL;
// 	return (node);
// }

// static void	add_or_update_env(t_env **env, char *key, char *value, t_shell *shell)
// {
// 	t_env	*cur;
// 	char	*kv[3];

// 	cur = *env;
// 	while (cur)
// 	{
// 		if (!ft_strcmp(cur->key, key))
// 		{
// 			if (value)
// 			{
// 				free(cur->value);
// 				cur->value = ft_strdup(value);
// 			}
// 			return ;
// 		}
// 		cur = cur->next;
// 	}
// 	kv[0] = key;
// 	if (value)
// 		kv[1] = value;
// 	else
// 		kv[1] = "";
// 	kv[2] = NULL;
// 	append_to_env_lst(env, create_node2(kv, shell));
// }

// static void	add_new_env(t_env **env, char *key, char *value, t_shell *shell)
// {
// 	char	*kv[3];

// 	kv[0] = key;
// 	kv[1] = value ? value : "";
// 	kv[2] = NULL;
// 	append_to_env_lst(env, create_node2(kv, shell));
// }

static t_env	*create_export_node(char *key, char *value, t_shell *shell)
{
	t_env	*node;
	char	*kv[3];

	kv[0] = key;
	kv[1] = value;
	kv[2] = NULL;
	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (ft_malloc_error("ft_export t_env allocation\n", shell), NULL);
	node->key = ft_strdup(key);
	if (!node->key)
		return (ft_malloc_error("ft_export t_env allocation\n", shell), NULL);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

static void	add_or_update_env(t_env **env, char *key, char *value, t_shell *shell)
{
	t_env	*existing;

	existing = env_find(*env, key);
	if (existing)
	{
		if (value)
		{
			free(existing->value);
			existing->value = ft_strdup(value);
		}
	}
	else
		append_to_env_lst(env, create_export_node(key, value, shell));
		// add_new_env(env, key, value, shell);
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

static int	process_with_equals(char *arg, t_shell *shell)
{
	char	*sep;
	char	*identifier;

	sep = ft_strchr(arg, '=');
	*sep = '\0';
	identifier = arg;
	if (!is_valid_identifier(identifier))
	{
		if (sep == arg)
			print_export_error("=");
		else
			print_export_error(identifier);
		*sep = '=';
		return (1);
	}
	add_or_update_env(&(shell->env_lst), identifier, sep + 1, shell);
	*sep = '=';
	return (0);
}

static int	process_without_equals(char *arg, t_shell *shell)
{
	if (!is_valid_identifier(arg))
	{
		print_export_error(arg);
		return (1);
	}
	add_or_update_env(&(shell->env_lst), arg, NULL, shell);
	return (0);
}

static int	process_export_arg(char *arg, t_shell *shell)
{
	if (ft_strchr(arg, '='))
		return (process_with_equals(arg, shell));
	return (process_without_equals(arg, shell));
}

int	ft_export(char **argv, t_shell *shell)
{
	int	i;
	int	has_error;

	if (!argv[1])
		return (ft_export_env_list(shell->env_lst), 0);
	has_error = 0;
	i = 1;
	while (argv[i])
	{
		if (process_export_arg(argv[i], shell))
			has_error = 1;
		++i;
	}
	if (has_error)
		shell->exit = 1;
	return (has_error);
}
