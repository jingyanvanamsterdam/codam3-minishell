#ifndef PARSE_H
# define PARSE_H

# include "struct.h"
# include <stdlib.h>
// # include "pipe.h"
# include "executor.h"
# include <stdbool.h>
# include "utils.h"

//===========LEX======================
//helper funcs for lex
size_t	find_index(char *str, size_t len, char c);
size_t	find_end(char *str);
int		create_token_node(char *value, t_shell *shell, t_type type);

//Quote handling funcs.
size_t	find_close_quote(char *str, size_t start, size_t end, t_shell *shell);
size_t	quote_index(char *str, size_t end);

//Main funcs for lex
size_t	update_start(char *str, t_shell *shell);
size_t	handle_special_symbol(char *str, size_t end, t_shell *shell);
size_t	skip_space(char *str, size_t end);
int		tokenization(t_shell *shell);
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

//REDIR funcs 
//int		open_infile(char *file);
int		open_outfile(char *file);
int		output_append(char *file);

int		handle_redir_fd(t_shell *shell, t_redir *redir);
t_token	*handle_redir(t_token *token, t_redir **redir, t_shell *shell);
int	    handle_cmd_io(t_shell *shell, t_cmd *cmd);

// heredoc 
void	run_hd_process(bool q, char *deli, t_shell *shell, t_cmd *cmd);
int	do_hd_loop(bool q, char *delimiter, t_shell *shell, t_cmd *cmd);
int	heredoc(t_shell *shell, t_cmd *cmd, t_redir *redir, char *deli);
int     handle_cmd_heredoc(t_cmd *cmd, t_shell *shell);

//ENV_PATH AND PATH
char	**create_env_path(t_env *env_lst);
int	     set_tcmd_pathes(t_shell *shell);

//T_CMD creation funcs #5
void	update_cmd_redir(t_redir *redir, t_shell *shell);
int		update_cmds_arr(char **cmd, t_token *token, t_shell *shell);
int		init_cmd_node(t_shell *shell, size_t size);
size_t	calculate_cmd_len(t_token *token);

int	    finish_set_tcmd(t_shell *shell);
t_token	*parse_token(t_token *token, t_shell *shell, t_cmd *cmd);
int		parsing(t_shell *shell);

//====================================

#endif