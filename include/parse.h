#ifndef PARSE_H
# define PARSE_H

# include "struct.h"
# include <stdlib.h>
# include "pipe.h"
#include <stdbool.h>

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

//=====Funcs handle quotes for heredoc and handle normal token.=================
//Expands handling funcs. 
char	*lookup_key(t_env *env_lst, const char *key);
char	*expansion(char *str, t_shell *shell, size_t end);
size_t	handle_expands(char *str, t_shell *shell, t_quotok **tok);

//quote utils
size_t	check_quote(t_quote *status, char *str);
size_t	find_stop(char *str, t_quote *status);
int		create_quotok_node(char *value, t_quotok **head);

//quote main funcs.
t_quotok	*tokenize_quote(char *value, t_shell *shell, bool heredoc);
char		*join_quotok(t_quotok *quotok);
char		*remove_quote(char *value, t_shell *shell, bool hdoc);
//============

//REDIR funcs #2
t_token	*handle_redir(t_token *token, t_redir **redir, t_shell *shell);

//T_CMD creation funcs #5
void	update_cmd_redir(t_redir *redir, t_shell *shell);
void	update_cmds_arr(char **cmd, t_token *token, t_shell *shell);
void	init_cmd_node(t_shell *shell, char **cmd);
size_t	calculate_cmd_len(t_token *token);

//void	print_quotok(t_quotok *head)
t_token	*parse_token(t_token *token, t_shell *shell, char **cmd);
void	parsing(t_shell *shell);
//====================================

#endif