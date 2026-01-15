#include "minishell.h"
#include "struct.h"
#include <stdbool.h>
#include "libft.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <readline/readline.h>
#include <readline/history.h>

int	count_cmd(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

/**
 * close all fds (cmd's redireciton and pipes) before call this func
 * If it is exit of child process, free_pip_params will be run
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
