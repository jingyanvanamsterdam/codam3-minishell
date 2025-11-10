#ifndef LEX_H
# define LEX_H

#include "struct.h"

void	free_token_list(t_token **lst);
void	free_env_lst(t_env **lst);
void	free_split(char **key_value);

void	ft_malloc_failure(char *s, t_shell *shell);

t_env 	*init_env(char **envp, t_shell *shell);

t_token *tokenization(char *input, t_shell *shell);
#endif