#include "utils.h"
// for the ERROR defined in parse.h
#include "parse.h"
#include "libft.h"
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h> 
#define _GNU_SOURCE
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void	ft_shell_input_error(void)
{
	ft_putstr_fd("To run minishll, you can do: \n", 2);
	ft_putstr_fd("./minishll\n or\n", 2);
	ft_putstr_fd("./minishll -c \'commands\'", 2);
	exit(EXIT_FAILURE);
}

//不需要强制退出。最好的用户体验是：打印 "Memory error" -> 释放当前坏掉的命令内存 -> 重新显示 minishell$> 让用户输入下一条命令。 GNU C Library: Error Recovery 建议程序应尽可能尝试恢复而非直接崩溃。
void	ft_malloc_error(char *s, t_shell *shell)
{
	ft_putstr_fd(ERROR "minishell: " RESET, 2);
	ft_putstr_fd("malloc failure at ", 2);
	ft_putstr_fd(s, 2);
	shell->exit = 1;
}

void	ft_input_error(char *errmes, char *s, t_shell *shell)
{
	ft_putstr_fd(ERROR "minishell: syntax error: " RESET, 2);
	ft_putstr_fd(errmes, 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("\n", 2);
	if (shell->token)
		free_token_lst(&(shell->token));
	if (shell->cmd)
		free_cmd_lst((&shell->cmd));
}

/** Special malloc failure handle fo parsing; using shell->token to check failure. so need to free them here first */
void	ft_malloc_parsing(char *s, t_shell *shell)
{
	ft_malloc_error(s, shell);
	if (shell->token)
		free_token_lst(&(shell->token));
	if (shell->cmd)
		free_cmd_lst((&shell->cmd));
}

// To do check when the error is being printed if cannot find the file and there is heredoc.
void	ft_error_printing(char *mes)
{
	ft_putstr_fd(ERROR "minishell: " RESET, 2);
	ft_putstr_fd(mes, 2);
	write(2, ": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	write(2, "\n", 1);
}

// sh has no warning. bash has long warning message. decide whether do warning. 
void	ft_warning_printing(void)
{
	ft_putstr_fd(ERROR "minishell: warning: heredoc delimited by end of file\n" RESET, 2);
}

//不需要强制退出。fork 失败需要free所有的pipes if any 和pid
void	ft_pipe_error(t_shell *shell, char *str, int n)
{
	ft_error_printing(str);
	shell->exit = EXIT_FAILURE;
	close_cmd_fds(shell);
	if (n > 0)
		close_pipes_i(shell->pip_param, n);
	free_pip_param(shell, n);
}

void	ft_malloc_exe(char *s, t_shell *shell, int n)
{
	ft_malloc_error(s, shell);
	close_cmd_fds(shell);
	if (n > 0)
		close_pipes_i(shell->pip_param, n);
	free_pip_param(shell, n);
}


/**close_all_fds() make sure before calling this func, every fd is closed. 
 * free_pip_params will run if it is child process of pipes. 
*/
void	ft_process_exit(t_shell *shell, bool pt_exit)
{
	int	code;
	int	clear_history;

	code = shell->exit;
	clear_history = shell->interactive;
	free_shell(shell);
	if (clear_history)
		rl_clear_history();
	if (pt_exit)
		ft_putstr_fd("process exit\n", 1);
	exit(code);
}

void	close_fd(int *fd)
{
	if (*fd != -1 && *fd != 0 && *fd != 1 && *fd != 2)
		close(*fd);
	*fd = -1;
}

void	close_cmd_fds(t_shell *shell)
{
	t_cmd	*cmd;
	t_redir *redir;

	cmd = shell->cmd;
	while (cmd)
	{
		close_fd(&(cmd->fd[0]));
		close_fd(&(cmd->fd[1]));
		redir = cmd->redir;
		while (redir)
		{
			close_fd(&(redir->fd));
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

void	close_pipes_i(t_pipe *params, int n)
{
	int	i;

	if (!params || !params->pipes)
		return ;
	i = 0;
	while (i < n)
	{
		close_fd(&(params->pipes[i][0]));
		close_fd(&(params->pipes[i][1]));
		++i;
	}
	return ;
}

/** make sure close all the fds before calling this func */
void	ft_reset_shell(t_shell *shell)
{
	if (shell->input)
		free_charptr(&(shell->input));
	if (shell->token)
		free_token_lst(&(shell->token));
	if (shell->cmd)
		free_cmd_lst(&(shell->cmd));
	shell->prev_exit = shell->exit;
	shell->exit = 0;
}
