#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

int	ft_pwd(char **argv, t_env *env);
int	ft_echo(char **argv, t_env *env);
int	ft_env(char **argv, t_env *env);

#endif