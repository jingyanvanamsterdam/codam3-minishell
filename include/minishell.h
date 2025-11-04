#ifndef MINISHELL_H
# define MINISHELL_H

# include "struct.h"

//暂时先放所有的function

// ft_free 
void	free_split(char **key_value);
void	free_env_list(t_env *env_list); 


// error_handle.c
void	ft_malloc_failure(char	*s, t_env *env_list);

//-------Initialization:
//init env:
t_env	*create_node(char **key_value, t_env *env_list);
t_env	*init_env(char **envp);

#endif