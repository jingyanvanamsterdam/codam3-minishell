/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 14:37:08 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 14:50:21 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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
