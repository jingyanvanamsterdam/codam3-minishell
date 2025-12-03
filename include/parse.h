#ifndef PARSE_H
# define PARSE_H

# include "struct.h"
# include <stdlib.h>
# include "pipe.h"

# define RESET   "\033[0m"
# define BLACK   "\033[30m"
# define ERROR   "\033[31m" // RED
# define EXEC    "\033[32m" // GREEN
//# define YELLOW  "\033[33m"
//# define BLUE    "\033[34m"
//# define MAGENTA "\033[35m"
//# define CYAN    "\033[36m"
//# define WHITE   "\033[37m"

//UTILS
void	init_env(char **envp, t_shell *shell);
//===========LEX======================
//helper funcs for lex
char	*append_to_str(char *dst, char *src);
size_t	find_index(char *str, size_t len, char c);
size_t	find_end(char *str);
void	create_token_node(char *value, t_shell *shell, t_type type);

//Quote handling funcs.
size_t	find_close_quote(char *str, size_t start, size_t end, t_shell *shell);
size_t	quote_index(char *str, size_t end);

//Main funcs for lex
size_t	update_start(char *str, t_shell *shell);
size_t	handle_special_symbol(char *str, size_t end, t_shell *shell);
size_t	skip_space(char *str, size_t end);
void	tokenization(char *input, t_shell *shell);
//====================================

//=============PARSE==================

//Expands handling funcs. 
size_t	skip_expansion(char *str);
char	*lookup_key(t_env *env_lst, const char *key);
char	*expansion(char *str, t_shell *shell, size_t end);
char	*process_expand(char *str, size_t *i, size_t *prev, t_shell *shell);
char	*handle_expands(char *str, size_t len, t_shell *shell);

//helper functions for parsing commands
size_t	calculate_cmd_len(t_token *token);
void	append_to_cmd(char **cmd, t_token *token, t_shell *shell);
void	create_cmd_node(t_shell *shell, char **cmd);

//helper funcs
void	update_cmd_redir(t_redir *redir, t_shell *shell);
char	*handle_dquote(char *str, size_t len, t_shell *shell);
char	*handle_token(t_type t, t_token *token, t_shell *shell);
t_token	*handle_redir(t_token *token, t_redir **redir, t_shell *shell);

t_token	*parse_token(t_token *token, t_shell *shell, char **cmd);
void	parsing(t_shell *shell);
//====================================

#endif