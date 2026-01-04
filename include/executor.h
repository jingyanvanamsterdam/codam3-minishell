#ifndef EXECUTOR_H
# define EXECUTOR_H

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

// executor.c
void	executor(t_shell *shell);
int		count_cmd(t_cmd *cmd);
void	pipes_initializer(t_pipe *params);
void	close_pipes(t_pipe *params);
void	apply_redirection(t_redir *r);
void	child_process(t_shell *shell, t_cmd *cmd, t_pipe *param, int i);
void	pipe_executor(t_shell *shell, t_pipe *params);
void	wait_handler(t_shell *shell, t_pipe *param);

// builtins_handler.c
int		is_builtin(char *command);
void	execve_builtin(t_shell *shell, int command_type, t_cmd *cmd);

#endif