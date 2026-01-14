#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

// Is it supposed to return int to indicate success or failure?
// Or just void?
int	ft_cd(char **argv, t_shell *shell);
int	ft_pwd(char **argv, t_shell *shell);
int	ft_echo(char **argv, t_shell *shell);
int	ft_env(char **argv, t_shell *shell);

int	ft_unset(char **argv, t_shell *shell);
int	ft_exit(char **argv, t_shell *shell);
int	ft_export(char **argv, t_shell *shell);

#endif