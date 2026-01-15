#include "minishell.h"
#include <stdio.h>
#include "libft.h"
#include <error.h>
#include <fcntl.h>
#include "utils.h"
#include "env.h"
#include "parse.h"
#include "struct.h"
#include <stdlib.h>		// for EXIT_FAILURE
#include <sys/wait.h>
#include <unistd.h>		// for STDIN_FILENO
#include "libft.h"
#include <errno.h>

void	setup_stream(int stream[2], t_cmd *cmd, int i, t_shell *shell)
{
	int	count;
	int	**pipes;

	count = shell->pip_param->cmd_count;
	pipes = shell->pip_param->pipes;
	// Set input stream
	if (i != 0 && cmd->fd[0] == 0 && pipes)
		stream[0] = pipes[i - 1][0];
	else 
		stream[0] = cmd->fd[0];
	// Set output stream
	if (i < (count - 1) && cmd->fd[1] == 1 && pipes)
		stream[1] = pipes[i][1];
	else
		stream[1] = cmd->fd[1];
}

int	dup_files(t_shell *shell, int stream[2])
{
	if (dup2(stream[0], STDIN_FILENO) < 0)
	{
		shell->exit = 1;
		ft_error_printing("fail at dup2");
	}
	if (dup2(stream[1], STDOUT_FILENO) < 0)
	{
		shell->exit = 1;
		ft_error_printing("fail at dup2");
	}	
	close_cmd_fds(shell);
	close_pipes_i(shell->pip_param, shell->pip_param->cmd_count - 1);
	return (shell->exit);
}
