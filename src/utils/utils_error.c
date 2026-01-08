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
void	ft_malloc_failure(char *s, t_shell *shell)
{
	int	clear_history;

	clear_history = shell->interactive;
	ft_putstr_fd(ERROR "minishell: " RESET, 2);
	ft_putstr_fd("malloc failure at ", 2);
	ft_putstr_fd(s, 2);
	free_shell(shell);
	if (clear_history)
		rl_clear_history();
	// ft_reset_shell(shell, pipe_idx);
	exit(EXIT_FAILURE);
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

//不需要强制退出。fork 失败需要free所有的pipe，
void	ft_pipe_error(t_shell *shell, char *str, int n)
{
	ft_error_printing(str);
	shell->exit = EXIT_FAILURE;
	close_all_fds(shell, n);
	ft_reset_shell(shell, n);
}

//void	ft_process_error_exit(t_shell *shell, char *mes)
//{
//	int	code;
//	int	clear_history;

//	code = shell->exit;
//	clear_history = shell->interactive;
//	ft_error_printing(mes);
//	close_all_fds(shell, shell->pip_param->cmd_count);
//	free_shell(shell);
//	if (clear_history)
//		rl_clear_history();
//	exit(code);
//}

/**close_all_fds() make sure before calling this func, every fd is closed. */
void	ft_process_exit(t_shell *shell)
{
	int	code;
	int	clear_history;

	code = shell->exit;
	clear_history = shell->interactive;
	free_shell(shell);
	if (clear_history)
		rl_clear_history();
	exit(code);
}

void	close_fd(int *fd)
{
	if (*fd != -1 && *fd != 0 && *fd != 1)
		close(*fd);
	*fd = -1;
}

void	close_all_fds(t_shell *shell, int pipe_idx)
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
	if (shell->pip_param && pipe_idx != 0)
		close_pipes_i(shell->pip_param, pipe_idx);
}
/** make sure close all the fds before calling this func */
void	ft_reset_shell(t_shell *shell, int pipe_idx)
{
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->token)
		free_token_lst(&(shell->token));
	if (shell->cmd)
		free_cmd_lst(&(shell->cmd));
	if (shell->pip_param)
	{
		if (shell->pip_param->pipes)
			free_pipes_n(shell->pip_param->pipes, pipe_idx);
		if (shell->pip_param->pids)
		{
			free(shell->pip_param->pids);
			shell->pip_param->pids = NULL;
		}
	}
	shell->prev_exit = shell->exit;
	shell->exit = 0;
}
