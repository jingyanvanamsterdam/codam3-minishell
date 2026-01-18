/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 14:25:14 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 16:37:11 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

// Is it supposed to return int to indicate success or failure?
// Or just void?
int		ft_cd(char **argv, t_shell *shell);
int		ft_pwd(char **argv, t_shell *shell);
int		ft_echo(char **argv, t_shell *shell);
int		ft_env(char **argv, t_shell *shell);
int		ft_unset(char **argv, t_shell *shell);
int		ft_exit(char **argv, t_shell *shell);
int		ft_export(char **argv, t_shell *shell);

// ft_export_helper.c
int		is_valid_identifier(const char *identifier);
void	ft_export_env_list(t_env *env);
int		process_with_equals(char *arg, t_shell *shell);
int		process_without_equals(char *arg, t_shell *shell);
int		process_export_arg(char *arg, t_shell *shell);

#endif