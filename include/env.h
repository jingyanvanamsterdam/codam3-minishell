#ifndef ENV_H
# define ENV_H

# include "struct.h"

// ======= env_error.c =======
void	ft_malloc_failure(char	*s, t_env *env_list);

// ======= env_free.c =======
void	free_env_list(t_env *env_list);

// ======= env_getset.c =======
void	env_set(t_env **env, const char *key, const char *value);
void	env_unset(t_env **env, const char *key);

//  ======= env_init.c =======
t_env	*create_node(char **key_value, t_env *env_list);
t_env	*init_env(char **envp);

//  ======= env_utils.c =======
t_env	*env_find(t_env *env, const char *key);
char    *env_get_value(t_env *env, const char *key);
int		env_key_exists(t_env *env, const char *key);
char	**env_to_array(t_env *env);

//  ======= free_split.c =======
void	free_split(char **key_value);


#endif