/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils_error_2.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: kuyu <kuyu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/01/18 16:34:09 by kuyu          #+#    #+#                 */
/*   Updated: 2026/01/18 17:56:43 by jdong         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"

void	ft_builtin_error(char *cmd, char *arg, char *msg)
{
	ft_putstr_fd(ERROR "minishell: " RESET, 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}
