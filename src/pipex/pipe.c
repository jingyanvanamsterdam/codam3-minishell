#include "pipe.h" // change to minishell
#include "parse.h"
#include "struct.h"
#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> 
#include <sys/types.h>
#include <sys/wait.h>

extern char		**environ; // delete and replace with shell->env_lst later. 

int	dup_files(t_shell *shell, int *file, int **pipes, int i)
{
	int	count;

	redirect_fd(file, pipes, i, shell);
	count = count_cmd(shell->cmd);
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

void	execve_cmd(t_shell *shell, int i)
{
	t_cmd *cmd;
	int j;

	j = 0;
	cmd = shell->cmd;
	while (j++ < i)
		cmd = cmd->next;
	if (!cmd->cmd)
	{
		shell->exit = 0;
		return ;
	}
	if (!cmd->path)
	{
		shell->exit = EXIT_NOCMD;
		return ;
	}
	//env_lst is a list, but execve need a char **enviorn
	if (execve(cmd->path, cmd->cmd, environ) == -1)
	{
		ft_error_printing(cmd->cmd[0]);
		shell->exit = EXIT_CMD_NOEXC;
	}
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

//void	run_child_process(t_shell *shell, int *file, int **pipes, int i)
//{
//	setup_child_signal();
//	if (dup_files(shell, file, pipes, i) == 0)
//		execve_cmd(shell, i);
//	ft_free_exit(pipes, shell);
//}

int	create_process(t_shell *shell, int **pipes, int	*file)
{
	int		i;
	pid_t	pid;
	int		count;

	i = 0;
	count = count_cmd(shell->cmd);
	while (i < count)
	{
		pid = fork();
		if (pid < 0)
			return (perror("sh: fork"), -1);
		else if (pid == 0)
		{
			//run_child_process
			setup_child_signal();
			printf("set child singal\n");
			if (dup_files(shell, file, pipes, i) == 0)
				execve_cmd(shell, i);
			ft_free_exit(pipes, shell);
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
	int	status;
	// handle cmd execution path. 
	count = count_cmd(shell->cmd);
	pipes = NULL;
	if (count > 1)
		pipes = create_pipes(shell);
	// handle redirection including heredoc.
	file[0] = 0;
	file[1] = 1;
	pid = create_process(shell, pipes, file);
	if (pid == -1)
	{
		close_pipes(pipes, count - 1);
		shell->exit = 1;
	}
	else if (waitpid(pid, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			shell->exit = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit = 128 + WTERMSIG(status);
	}
	while (wait(NULL) > 0)
		;
	ft_free_exit(pipes, shell);
	printf("only for use of pid. %d", pid);
}

/** Question:
 * if the main process open or deal the redirection process, does it mean there will be only one input and one output file or source?
 * if in the process, there is other redirection, will be the latest redirection being considered as the redirection source?

 */