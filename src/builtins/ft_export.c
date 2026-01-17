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
	char	*original_arg;

	original_arg = ft_strdup(arg);
	if (!original_arg)
		return (1);
	sep = ft_strchr(arg, '=');
	*sep = '\0';
	identifier = arg;
	if (!is_valid_identifier(identifier))
	{
		if (sep == arg)
			ft_builtin_error("export: `", "=", "': not a valid identifier");
		else
			ft_builtin_error("export: `", original_arg, "': not a valid identifier");
		*sep = '=';
		free(original_arg);
		return (1);
	}
	update_env_value(shell, identifier, sep + 1);
	// add_or_update_env(&(shell->env_lst), identifier, sep + 1, shell);
	*sep = '=';
	free(original_arg);
	return (0);
}

static int	process_without_equals(char *arg, t_shell *shell)
{
	if (!is_valid_identifier(arg))
	{
		ft_builtin_error("export: `", arg, "': not a valid identifier");
		return (1);
	}
	update_env_value(shell, arg, NULL);
	// add_or_update_env(&(shell->env_lst), arg, NULL, shell);
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
