/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 15:35:26 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 15:36:36 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include "libft.h"

char	**split_env_with_equals(char *env_str,
	char *equals_pos, t_shell *shell)
{
	char	**key_value;

	key_value = (char **)malloc(sizeof(char *) * 3);
	if (!key_value)
		return (ft_malloc_error("setup env key_value", shell), NULL);
	key_value[0] = ft_substr(env_str, 0, equals_pos - env_str);
	key_value[1] = ft_strdup(equals_pos + 1);
	key_value[2] = NULL;
	if (!key_value[0] || !key_value[1])
	{
		if (key_value[0])
			free_charptr(&key_value[0]);
		if (key_value[1])
			free_charptr(&key_value[1]);
		free(key_value);
		return (ft_malloc_error("setup env key_value", shell), NULL);
	}
	return (key_value);
}

char	**split_env_without_equals(char *env_str, t_shell *shell)
{
	char	**key_value;

	key_value = (char **)malloc(sizeof(char *) * 2);
	if (!key_value)
		return (ft_malloc_error("setup env key_value", shell), NULL);
	key_value[0] = ft_strdup(env_str);
	key_value[1] = NULL;
	if (!key_value[0])
	{
		free(key_value);
		return (ft_malloc_error("setup env key_value", shell), NULL);
	}
	return (key_value);
}

char	**split_env_string(char *env_str, t_shell *shell)
{
	char	*equals_pos;

	equals_pos = ft_strchr(env_str, '=');
	if (equals_pos)
		return (split_env_with_equals(env_str, equals_pos, shell));
	return (split_env_without_equals(env_str, shell));
}

int	process_env_entry(char *env_str, t_shell *shell)
{
	char	**key_value;
	t_env	*node;

	key_value = split_env_string(env_str, shell);
	if (!key_value)
		return (0);
	node = create_node(key_value, shell);
	if (!node)
		return (free_2d_arr(key_value), 0);
	append_to_env_lst(&(shell->env_lst), node);
	free_2d_arr(key_value);
	return (1);
}

void	create_minimal_env(t_shell *shell)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	if (cwd)
		update_env_value(shell, "PWD", cwd);
	free_charptr(&cwd);
	update_env_value(shell, "SHLVL", "1");
	update_env_value(shell, "PATH",
		"/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
}
