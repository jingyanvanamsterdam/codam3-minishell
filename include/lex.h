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

void 	init_env(char **envp, t_shell *shell);

//helper funcs for lex
char	*append_to_str(char *dst, char *src);
void	append_to_lst(t_token **head, t_token *node);
size_t	find_index(char *str, size_t end, char c);
size_t	find_end(char *str);
void	create_token_node(char *value, t_shell *shell, t_token_type type);

//Expands handling funcs. 
size_t	skip_expansion(char *str);
char	*lookup_key(t_env *env_lst, const char *key);
char	*expansion(char *str, t_shell *shell, size_t end);
char	*handle_expands(char *str, size_t len, t_shell *shell);

//Quote handling funcs.
size_t	find_close_quote(char *str, size_t start, size_t end, t_shell *shell);
size_t	quote_index(char *str, size_t end, t_lex_status *status);
char	*handle_quote(char *str, size_t len, t_shell *shell);

//Main funcs for lex
size_t	update_start(char *str, t_shell *shell);
size_t	handle_special_symbol(char *str, size_t end, t_shell *shell);
size_t	skip_space_or_quotes(char *str, size_t end);
void	tokenization(char *input, t_shell *shell);

#endif