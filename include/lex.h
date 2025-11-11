#ifndef LEX_H
# define LEX_H

#include "struct.h"
#include <stdbool.h> // for boolean

void	free_token_list(t_token **lst);
void	free_env_lst(t_env **lst);
void	free_shell(t_shell *shell);
void	free_split(char **key_value);

void	ft_malloc_failure(char *s, t_shell *shell);

t_env 	*init_env(char **envp, t_shell *shell);

t_token *tokenization(char *input, t_shell *shell);

//Expands handling funcs. 
bool	contain_expands(char *str, int end);
char	*check_expands(char *str, int len, t_shell *shell);
//Utils for expands:
char	*append_to_str(char *dst, char *src);
int		skip_expansion(char *str);
char	*lookup_key(t_env *env_lst, const char *key);
char	*expansion(char *str, t_shell *shell, int end);
char	*join_prev_expands(char *str, int start, int cur, t_shell *shell, char *res);


#endif