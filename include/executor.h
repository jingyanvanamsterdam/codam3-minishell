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

// execute.c
void	setup_stream(int stream[2], t_cmd *cmd, int i, t_shell *shell);
int		dup_files(t_shell *shell, int stream[2]);
void	execve_cmd(t_shell *shell, t_cmd *cmd);
void	find_file_redir(t_cmd *cmd);
int		apply_redir_parent(t_shell *shell, int savefd[2]);
void	restore_parent_fd(int saved_stdfd[2]);
int		single_builtin_handler(t_shell *shell);
int		create_pipes(t_shell *shell);
//void	parent_close_file(t_shell *shell, int i);
void	run_child_process(t_shell *shell, t_cmd *cmd, int stream[2]);
int		create_process(t_shell *shell);
void	wait_handler(t_shell *shell);
void	executor(t_shell *shell);

// builtins_handler.c
int		is_builtin(char *command);
void	execve_builtin(t_shell *shell, int command_type, t_cmd *cmd);

// ft_echo_test.c
int		ft_echo_test(char **argv, t_shell *shell);


// signal:
void	handle_sigint(int sig);
void	sig_interactive(void);
void	sig_noninteractive(void);
void	sig_exe_child(void);
void	sig_heredoc(void);

void	non_interactive_c(t_shell *shell, char *av);
void	non_interactive_no_c(t_shell *shell, char **av);

int	process_input(t_shell *shell);

#endif