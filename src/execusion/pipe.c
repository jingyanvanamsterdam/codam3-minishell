#include "pipe.h" // change to minishell
#include "struct.h"
#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int	dup_files(t_shell *shell, int *file, int **pipes)
{
	int	count;

	count = count_cmd(shell->cmd);
	handle_redir(shell, file, pipes);
	if (file[0] == -1 || file[1] == -1)
		return (shell->exit);
	if (dup2(file[0], STDIN_FILENO) < 0)
		ft_error_printing("dup2 input fail");
	if (dup2(file[1], STDOUT_FILENO) < 0)
	{
		ft_error_printing("dup2 output fail");
		shell->exit = 1;
		close_pipes(pipes, (count - 1));
		close_files(file);
		return (shell->exit);
	}
	close_pipes(pipes, (count - 1));
	close_files(file);
	return (shell->exit);
}


int	**create_pipes(t_shell *shell)
{
	int	i;
	int	**pipes;
	int	count;

	i = 0;
	count = count_cmd(shell->cmd);
	pipes = malloc(sizeof(int *) * (count - 1));
	if (!pipes)
		ft_malloc_failure("pipe malloc failure\n", shell);
	while (i < (count - 1))
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			close_pipes(pipes, i);
			ft_pipe_error(shell, "pipe: ", pipes, i);
		}
		i++;
	}
	return (pipes);
}

int	create_process(t_shell *shell, int **pipes, int	*file)
{
	int		i;
	pid_t	pid;
	int		code;
	int		count;

	i = 0;
	code = 0;
	count = count_cmd(shell->cmd);
	while (i < count)
	{
		pid = fork();
		if (pid < 0)
			return (perror("sh: fork"), -1);
		else if (pid == 0)
		{
			init_file(file, pipes, i, shell);
			if (dup_files(shell, file, pipes) == 0)
				execve_cmd(shell, i);
			code = shell->exit;
			ft_free_exit(pipes, shell, code);
		}
		else
			parent_close_file(shell, i, pipes, file);
		i++;
	}
	return (pid);
}

void	execusion(t_shell *shell)
{
	int	count;
	int	**pipes;
	int	pid;
	int	file[2];
	// handle cmd execution path. 

	// handle redirection including heredoc.
	pipes = create_pipes(shell);
	file[0] = 0;
	file[1] = 1;
	pid = create_process(shell, pipes, file);
}

/** Question:
 * if the main process open or deal the redirection process, does it mean there will be only one input and one output file or source?
 * if in the process, there is other redirection, will be the latest redirection being considered as the redirection source?

 */