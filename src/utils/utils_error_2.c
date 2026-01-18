/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 16:34:09 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 16:35:33 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

// "export A-"
// ft_builtin_error("export: ", identifier, "': not a valid identifier");
// minishell: export: `A-': not a valid identifier
void	ft_builtin_error(char *cmd, char *arg, char *msg)
{
	ft_putstr_fd(ERROR "minishell: " RESET, 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}

// static void	print_export_error(char *identifier)
// {
// 	ft_putstr_fd("export: `", 2);
// 	ft_putstr_fd(identifier, 2);
// 	ft_putstr_fd("': not a valid identifier\n", 2);
// }