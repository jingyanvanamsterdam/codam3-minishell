/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util_malloc_error.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: kuyu <kuyu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/01/18 16:34:03 by kuyu          #+#    #+#                 */
/*   Updated: 2026/01/18 17:55:47 by jdong         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "libft.h"

/** shell->exit = 1 in this func */
void	ft_malloc_error(char *s, t_shell *shell)
{
	ft_putstr_fd(ERROR "minishell: " RESET, 2);
	ft_putstr_fd("malloc failure at ", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("\n", 2);
	shell->exit = 1;
}

/** Special malloc failure handle for parsing;
 * using shell->token to check failure. so need to free them here first */
void	ft_malloc_parsing(char *s, t_shell *shell)
{
	ft_malloc_error(s, shell);
	if (shell->token)
		free_token_lst(&(shell->token));
	if (shell->cmd)
		free_cmd_lst((&shell->cmd));
}

/** Special malloc fail handle for pip_params */
void	ft_malloc_exe(char *s, t_shell *shell, int n)
{
	ft_malloc_error(s, shell);
	close_cmd_fds(shell);
	if (n > 0)
		close_pipes_i(shell->pip_param, n);
	free_pip_param(shell, n);
}
