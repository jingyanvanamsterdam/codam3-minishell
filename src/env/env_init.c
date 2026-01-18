/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 15:30:03 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 15:35:49 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include "libft.h"

int	init_env(char **envp, t_shell *shell)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (!process_env_entry(envp[i], shell))
			return (0);
		i++;
	}
	if (envp[0] == NULL)
		create_minimal_env(shell);
	return (1);
}
