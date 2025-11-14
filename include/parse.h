#ifndef PARSE_H
# define PARSE_H

# include "struct.h"
# include <stdlib.h>

//UTILS
void	free_2d_arr(char **arr);
void	free_token_lst(t_token **lst);
void	free_env_lst(t_env **lst);
void	free_redir_lst(t_redir **lst);
void	free_cmd_lst(t_cmd **lst);
void	free_shell(t_shell *shell);

void	init_env(char **envp, t_shell *shell);

void	ft_input_error(char	*s, t_shell *shell);
void	ft_malloc_failure(char *s, t_shell *shell);

//===========LEX======================
//helper funcs for lex
char	*append_to_str(char *dst, char *src);
size_t	find_index(char *str, size_t end, char c);
size_t	find_end(char *str);
void	create_token_node(char *value, t_shell *shell, t_type type);

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
//====================================

//=============PARSE==================

//helper funcs
size_t	calculate_cmd_len(t_token *token);
void	append_to_cmd(char **cmd, char *src, t_shell *shell);
t_token	*handle_redir(t_type t, t_token *token, t_redir **redir, t_shell *shell);
void	create_cmd_node(t_redir *redir, t_shell *shell, char **cmd);

void	parsing(t_shell *shell);
//====================================

#endif