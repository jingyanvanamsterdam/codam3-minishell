/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 14:36:58 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 16:48:41 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "libft.h"
#include "minishell.h"

static char	*get_target_directory(char **argv, t_shell *shell)
{
	t_env	*target;

	if (!argv[1])
	{
		target = env_find(shell->env_lst, "HOME");
		if (!target)
			ft_builtin_error("cd: ", "HOME", " not set");
		return (target->value);
	}
	else if (!ft_strcmp(argv[1], "-"))
	{
		target = env_find(shell->env_lst, "OLDPWD");
		if (!target)
			ft_builtin_error("cd: ", "OLDPWD", " not set");
		ft_putstr_fd(target->value, 1);
		ft_putstr_fd("\n", 1);
		return (target->value);
	}
	return (argv[1]);
}

static int	update_pwd_variables(t_shell *shell, char *oldpwd)
{
	char	*newpwd;

	update_env_value(shell, "OLDPWD", oldpwd);
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		return (1);
	update_env_value(shell, "PWD", newpwd);
	free_charptr(&newpwd);
	return (0);
}

int	ft_cd(char **argv, t_shell *shell)
{
	char	*target;
	char	*oldpwd;

	if (argv[2])
		return (ft_builtin_error("cd: ", "", "too many arguments"), 1);
	target = get_target_directory(argv, shell);
	if (!target)
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		oldpwd = ft_strdup("");
	if (chdir(target) == -1)
	{
		free_charptr(&oldpwd);
		return (ft_builtin_error("cd: ", target,
				": No such file or directory"), 1);
	}
	if (update_pwd_variables(shell, oldpwd) == 1)
	{
		free_charptr(&oldpwd);
		return (1);
	}
	free_charptr(&oldpwd);
	return (0);
}
