#ifndef UTILS_H
# define UTILS_H

# include "struct.h"

// utils_error.c
void	ft_shell_input_error(void);
void	ft_malloc_error(char *s, t_shell *shell);
void	ft_malloc_parsing(char *s, t_shell *shell);
void	ft_malloc_exe(char *s, t_shell *shell, int n);
void	ft_malloc_failure(char *s, t_shell *shell);
void	ft_input_error(char *errmes, char *s, t_shell *shell);
void	ft_error_printing(char *mes);
void	ft_warning_printing(void);
void	ft_pipe_error(t_shell *shell, char *str, int n);
void	ft_process_exit(t_shell *shell);
// close fd
void	close_fd(int *fd);
void	close_all_fds(t_shell *shell, int pipe_idx);
void	ft_reset_shell(t_shell *shell, int pipe_idx);

// utils_free.c
void	free_2d_arr(char **arr);
void	free_token_lst(t_token **lst);
void	free_env_lst(t_env **lst);
void	free_redir_lst(t_redir **lst);
void	free_cmd_lst(t_cmd **lst);
void	free_shell(t_shell *shell);
void	free_quotok(t_quotok **lst);
//void	free_pipes(t_pipe *params);
void	free_pipes_n(t_pipe *params, int count);
void	free_charptr(char **ptr);

// utils_helper.c
int		count_cmd(t_cmd *cmd);

#endif