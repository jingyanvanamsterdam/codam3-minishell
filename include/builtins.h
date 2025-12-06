#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

int	ft_pwd(char **argv, t_shell *shell);
int	ft_echo(char **argv, t_shell *shell);
int	ft_env(char **argv, t_shell *shell);

int	ft_unset(char **argv, t_shell *shell);
int	ft_exit(char **argv, t_shell *shell);
int	ft_export(char **argv, t_shell *shell);

#endif