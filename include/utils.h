#ifndef UTILS_H
# define UTILS_H

# include "struct.h"
# include <stdbool.h>

# define RESET   "\033[0m"
# define BLACK   "\033[30m"
# define ERROR   "\033[31m" // RED
# define EXEC    "\033[32m" // GREEN
//# define YELLOW  "\033[33m"
//# define BLUE    "\033[34m"
//# define MAGENTA "\033[35m"
//# define CYAN    "\033[36m"
//# define WHITE   "\033[37m"

// utils_malloc_error.c
void	ft_malloc_error(char *s, t_shell *shell);
void	ft_malloc_parsing(char *s, t_shell *shell);
void	ft_malloc_exe(char *s, t_shell *shell, int n);

// utils_error.c
void	ft_execve_error(char *cmd_name, char *err_mes);
void	ft_input_error(char *errmes, char *s, t_shell *shell);
void	ft_error_printing(char *mes);
void	ft_warning_printing(void);
void	ft_pipe_error(t_shell *shell, char *str, int n);

// utils_error_2.c
void	ft_builtin_error(char *cmd, char *arg, char *msg);

// utils_helper.c
int		count_cmd(t_cmd *cmd);
void	ft_process_exit(t_shell *shell, bool pt_exit);
void	ft_reset_shell(t_shell *shell);

// utils_free.c
void	free_2d_arr(char **arr);
void	free_pipes_n(t_pipe *params, int count);
void	free_charptr(char **ptr);
void	free_pip_param(t_shell *shell, int	n);
void	free_shell(t_shell *shell);

// utils_free_lst.c
void	free_token_lst(t_token **lst);
void	free_env_lst(t_env **lst);
void	free_redir_lst(t_redir **lst);
void	free_cmd_lst(t_cmd **lst);
void	free_quotok(t_quotok **lst);

// utils_close.c
void	close_fd(int *fd);
void	close_cmd_fds(t_shell *shell);
void	close_pipes_i(t_pipe *params, int n);

#endif