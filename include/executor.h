/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 14:25:24 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 18:09:15 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# define _GNU_SOURCE
# include "struct.h"
# include <signal.h>
# include <stdbool.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

# define ERROR_FILE 1
# define ERROR_SYS 2
# define ERROR_CMD 3
# define EXIT_CMD_NOEXC 126
# define EXIT_NOCMD 127
# define HD_NAME "/tmp/minishell-heredoc-"

extern volatile sig_atomic_t	g_sig;

// signal:
void		handle_sigint(int sig);
void		sig_interactive(void);
void		sig_noninteractive(void);
void		sig_exe_child(void);
void		sig_heredoc(void);

// setup_main.c
void		init_shell(t_shell *shell, char **envp);
void		non_interactive_no_c(t_shell *shell, char **av);
void		non_interactive_c(t_shell *shell, char *av);
void		interactive_shell(t_shell *shell);

//setup_helper.c
int			is_script(char **env_paths, char *file, t_shell *shell);
int			open_argv_fd(char *file, t_shell *shell);
int			process_input(t_shell *shell);
void		executor(t_shell *shell);

// exe_utils.c
void		find_file_redir(t_cmd *cmd);
int			open_files(t_cmd *cmd, t_redir *redir, t_type type);
t_builtin	is_builtin(char *command);
void		execve_builtin(t_shell *shell, t_builtin type, t_cmd *cmd);
void		execve_cmd(t_shell *shell, t_cmd *cmd);

// builtin_exe.c
int			apply_redir_parent(t_shell *shell, int savefd[2]);
void		restore_parent_fd(int saved_stdfd[2]);
void		single_builtin_handler(t_shell *shell, t_builtin type);

// pipe_helpers.c
int			setup_stream(int stream[2], t_cmd *cmd, int i, t_shell *shell);
int			dup_files(t_shell *shell, int stream[2]);

//pipe_exe.c
int			create_pipes(t_shell *shell);
int			create_process(t_shell *shell);
void		run_child_process(t_shell *shell, t_cmd *cmd, int i);
void		wait_handler(t_shell *shell);

#endif