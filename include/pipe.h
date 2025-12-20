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

// pipe.c
void	executor(t_shell *shell);

// pipe_helper.c
int		count_cmd(t_cmd *cmd);

// pipes_handler.c
void	pipes_initializer(t_pipe *params);
void	pipe_handler(t_shell *shell, t_pipe *params);

// 

#endif