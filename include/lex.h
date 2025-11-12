#ifndef LEX_H
# define LEX_H

# include "struct.h"
# include <stdlib.h>

void	free_token_list(t_token **lst);
void	free_env_lst(t_env **lst);
void	free_shell(t_shell *shell);
void	free_split(char **key_value);

void	ft_malloc_failure(char *s, t_shell *shell);
void	ft_input_error(char	*s, t_shell *shell);

t_env 	*init_env(char **envp, t_shell *shell);

//functions for lex
char	*append_to_str(char *dst, char *src);
t_token *tokenization(char *input, t_shell *shell);

//Expands handling funcs. 
size_t	expands_index(char *str, size_t end);
size_t	skip_expansion(char *str);
char	*lookup_key(t_env *env_lst, const char *key);
char	*expansion(char *str, t_shell *shell, size_t end);
char	*handle_expands(char *str, size_t i, size_t len, t_shell *shell);

//Quote handling funcs.
size_t	find_close_quote(char *str, size_t end, t_shell *shell);

#endif