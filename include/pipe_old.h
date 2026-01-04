#ifndef PIPE_H
# define PIPE_H

# define _GNU_SOURCE
# include "struct.h"
# include <signal.h>
# include <stdbool.h>
# include <fcntl.h>
# include <stdlib.h>

# define ERROR_FILE 1
# define ERROR_SYS 2
# define ERROR_CMD 3
# define EXIT_CMD_NOEXC 126
# define EXIT_NOCMD 127
# define HD_NAME "/tmp/minishell-heredoc-"

extern volatile sig_atomic_t g_sig;  // 只声明，不定义

//signal:
void	handle_sigint(int sig);
void	handle_sigint_child(int sig);
void	setup_signal(void);
void	setup_child_signal(void);

// helpers:
int		count_cmd(t_cmd *cmd);
void	redirect_fd(int *file, int **pipes, int i, t_shell *shell);
void	close_pipes(int **pipes, int n);
void	close_files(t_cmd *cmd, int *file);
void	parent_close_file(t_shell *shell, int i, int **pipes, int *file);

// ERROR:
void	ft_input_error(char *errmes, char *s, t_shell *shell);
void	ft_malloc_failure(char *s, t_shell *shell);
// new error func:
void	ft_pipe_error(t_shell *shell, char *str, int **pipes, int n);
void	ft_error_printing(char *mes);
void	ft_warning_printing(void);

//FREE:
void	free_2d_arr(char **arr);
void	free_token_lst(t_token **lst);
void	free_env_lst(t_env **lst);
void	free_redir_lst(t_redir **lst);
void	free_cmd_lst(t_cmd **lst);
void	free_shell(t_shell *shell);
//new free func:
void	free_pipes(int **pipes, int n);
void	ft_free_exit_process(int **pipes, t_shell *shell);
void	free_quotok(t_quotok **lst);


//stream funcs
int		heredoc(t_shell *shell, t_redir *redir);

//main execusion funcs
int		dup_files(t_shell *shell, int *file, int **pipes, int i);
int		**create_pipes(t_shell *shell);
int		create_process(t_shell *shell, int **pipes, int	*file);
void	execusion(t_shell *shell);

// pipes handler
// void	pipes_initializer(t_pipeparams *params);


#endif