/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 14:48:41 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 18:04:51 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdio.h>

// First character must be a letter or underscore
// Remaining characters must be alphanumeric or underscore
int	is_valid_identifier(const char *identifier)
{
	int	i;

	if (!identifier || !identifier[0])
		return (0);
	if (!ft_isalpha(identifier[0]) && identifier[0] != '_')
		return (0);
	i = 1;
	while (identifier[i])
	{
		if (!ft_isalnum(identifier[i]) && identifier[i] != '_')
			return (0);
		++i;
	}
	return (1);
}

void	ft_export_env_list(t_env *env)
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

int	process_with_equals(char *arg, t_shell *shell)
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
			ft_builtin_error("export: `", original_arg,
				"': not a valid identifier");
		*sep = '=';
		free(original_arg);
		return (1);
	}
	update_env_value(shell, identifier, sep + 1);
	*sep = '=';
	free_charptr(&original_arg);
	return (0);
}

int	process_without_equals(char *arg, t_shell *shell)
{
	if (!is_valid_identifier(arg))
	{
		ft_builtin_error("export: `", arg, "': not a valid identifier");
		return (1);
	}
	update_env_value(shell, arg, NULL);
	return (0);
}

int	process_export_arg(char *arg, t_shell *shell)
{
	if (ft_strchr(arg, '='))
		return (process_with_equals(arg, shell));
	return (process_without_equals(arg, shell));
}
