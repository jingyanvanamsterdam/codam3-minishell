/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils_error.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kuyu <kuyu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/01/18 16:34:11 by kuyu          #+#    #+#                 */
/*   Updated: 2026/01/18 17:58:22 by jdong         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "libft.h"
#include <string.h>
#include <errno.h>
#include <unistd.h>

void	ft_execve_error(char *cmd_name, char *err_mes)
{
	ft_putstr_fd(ERROR "minishell: " RESET, 2);
	ft_putstr_fd(cmd_name, 2);
	write(2, ": ", 2);
	ft_putstr_fd(err_mes, 2);
	ft_putstr_fd("\n", 2);
}

// To do check when the error is being printed 
// if cannot find the file and there is heredoc.
void	ft_error_printing(char *mes)
{
	ft_putstr_fd(ERROR "minishell: " RESET, 2);
	ft_putstr_fd(mes, 2);
	write(2, ": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	write(2, "\n", 1);
}

void	ft_input_error(char *errmes, char *s, t_shell *shell)
{
	ft_putstr_fd(ERROR "minishell: syntax error: " RESET, 2);
	ft_putstr_fd(errmes, 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("\n", 2);
	if (shell->token)
		free_token_lst(&(shell->token));
	if (shell->cmd)
		free_cmd_lst((&shell->cmd));
}

// sh has no warning. bash has long warning message. decide whether do warning. 
void	ft_warning_printing(void)
{
	ft_putstr_fd(ERROR "minishell: warning: " RESET, 2);
	ft_putstr_fd(ERROR "heredoc delimited by end of file\n" RESET, 2);
}

void	ft_pipe_error(t_shell *shell, char *str, int n)
{
	ft_error_printing(str);
	shell->exit = EXIT_FAILURE;
	if (n > 0)
		close_pipes_i(shell->pip_param, n);
	free_pip_param(shell, n);
}
