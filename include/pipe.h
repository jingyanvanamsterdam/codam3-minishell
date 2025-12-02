#ifndef PIPE_H
# define PIPE_H

# include "struct.h"

//signal:
void	handle_sigint(int sig);

// helpers:
int		count_cmd(t_cmd *cmd);
void	init_file(int *file, int **pipes, int i, t_shell *shell);
void	close_pipes(int **pipes, int n);
void	close_files(int *file);
void	parent_close_file(t_shell *shell, int i, int **pipes, int *file);

// new error func:
void	ft_pipe_error(t_shell *shell, char *str, int **pipes, int n);
void	ft_error_printing(char *mes);

//new free func:
void	free_pipes(int **pipes, int n);


//stream funcs
int		open_infile(char *file);
int		open_outfile(char *file);
void	contain_quote(char *limiter);
int		heredoc(t_shell *shell, t_redir *redir);
void	handle_redir(t_shell *shell, int *file);

//main execusion funcs
int		dup_files(t_shell *shell, int *file, int **pipes);
int		**create_pipes(t_shell *shell);
int		create_process(t_shell *shell, int **pipes, int	*file);
void	execusion(t_shell *shell);

#endif