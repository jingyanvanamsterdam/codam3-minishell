#ifndef EXECUTOR_H
# define EXECUTOR_H

# define _GNU_SOURCE
# include "struct.h"
# include <signal.h>
# include <stdbool.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h> // for printf testing

# define ERROR_FILE 1
# define ERROR_SYS 2
# define ERROR_CMD 3
# define EXIT_CMD_NOEXC 126
# define EXIT_NOCMD 127
# define HD_NAME "/tmp/minishell-heredoc-"

extern volatile sig_atomic_t g_sig;  // 只声明，不定义

// void	executor_tmp(t_shell *shell);
// void	pipes_initializer(t_pipe *params);
// void	close_pipes(t_pipe *params);
// void	apply_redirection(t_redir *r);
// void	child_process(t_shell *shell, t_cmd *cmd, t_pipe *param, int i);
// void	pipe_executor(t_shell *shell, t_pipe *params);
// void	wait_handler(t_shell *shell, t_pipe *param);

// execute.c
void	setup_stream(int stream[2], t_cmd *cmd, int i, t_shell *shell);
int		dup_files(t_shell *shell, t_cmd *cmd, int stream[2]);
void	execve_cmd(t_shell *shell, t_cmd *cmd);
void	find_file_redir(t_cmd *cmd);
int		apply_redir_parent(t_shell *shell, int savefd[2]);
void	restore_parent_fd(int saved_stdfd[2]);
int		single_builtin_handler(t_shell *shell);
void	create_pipes(t_shell *shell);
void	parent_close_file(t_shell *shell, int i);
void	run_child_process(t_shell *shell, t_cmd *cmd, int stream[2]);
void	create_process(t_shell *shell);
void	wait_handler(t_shell *shell);
void	executor(t_shell *shell);
void	close_pipes(t_pipe *params);
void	close_pipes_i(t_pipe *params, int n);


// builtins_handler.c
int		is_builtin(char *command);
void	execve_builtin(t_shell *shell, int command_type, t_cmd *cmd);

// ft_echo_test.c
int		ft_echo_test(char **argv, t_shell *shell);

//jd_process.c
void	close_pipes_i(t_pipe *params, int n);
void	ft_pipe_error(t_shell *shell, char *str, int n);


#endif