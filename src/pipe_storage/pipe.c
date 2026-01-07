////#include "pipe.h" // change to minishell
//#include "parse.h"
//#include "struct.h"
//#include "libft.h"
//#include <stdio.h>	
////#include <stdlib.h>
////#include <fcntl.h>
////#include <string.h>
//#include <unistd.h>
////#include <errno.h> 
////#include <sys/types.h>
////#include <sys/wait.h>

/////** The function close the previous redirect fd if there is any. */
////void	find_file_redir(t_cmd *cmd, int i)
////{
////	t_redir *redir;

////	redir = cmd->redir;
////	while (redir)
////	{
////		if (redir->type == REDIR_IN || redir->type == HEREDOC)
////		{
////			if (cmd->fd[0] > 0)
////				close(cmd->fd[0]);
////			cmd->fd[0] = redir->fd;
////		}
////		else if (redir->type == REDIR_OUT || redir->type == APPEND)
////		{
////			if (cmd->fd[1] != -1 && cmd->fd[1]!= 1)
////				close(cmd->fd[1]);
////			cmd->fd[1] = redir->fd;
////		}
////		redir = redir->next;
////	}
////}

////void	setup_stream(int stream[2], t_cmd *cmd, int i, int count)
////{
////	if (i != 0 && cmd->fd[0] == 0)
////		stream[0] = pipes[i - 1][0];
////	else 
////		stream[0] = cmd->fd[0];
////	if (i == (count - 1) && cmd->fd[1] == 1)
////		stream[1] = pipes[i][1];
////	else
////		stream[1] = cmd->fd[1];
////}
/////** find file redir() is change in/out fd if there is redirection in each cmd
//// * After that set pipes when there is no redirection on the comd (aka fd == initial value 0 and 1)
//// * parent_close_file() not sure do it seperately like now or all at once after while.
//// * run_child_process() should be reintergrate with KL code.
////*/
////int	create_process(t_shell *shell, int **pipes)
////{
////	int		i;
////	pid_t	pid;
////	int		count;
////	t_cmd	*cmd;
////	int		stream[2]

////	i = -1;
////	count = count_cmd(shell->cmd);
////	cmd = shell->cmd;
////	while (++i < count)
////	{
////		find_file_redir(cmd);
////		setup_stream(stream, cmd, i, count);
////		pid = fork();
////		if (pid < 0)
////			return (perror("sh: fork"), -1);
////		else if (pid == 0)
////			run_child_process(shell, cmd, stream);
////		else
////			parent_close_file(shell, i, pipes);
////		cmd = cmd->next;
////	}
////	return (pid);
////}

////int	**create_pipes(t_shell *shell)
////{
////	int	i;
////	int	**pipes;
////	int	count;

////	i = 0;
////	count = count_cmd(shell->cmd);
////	pipes = malloc(sizeof(int *) * (count - 1));
////	if (!pipes)
////		ft_malloc_failure("pipe malloc failure\n", shell);
////	while (i < (count - 1))
////	{
////		pipes[i] = malloc(sizeof(int) * 2);
////		if (!pipes[i] || pipe(pipes[i]) == -1)
////		{
////			close_pipes(pipes, i);
////			ft_pipe_error(shell, "pipe: ", pipes, i);
////		}
////		i++;
////	}
////	return (pipes);
////}

//void	execusion(t_shell *shell)
//{
//	int	count;
//	int	**pipes;
//	int	pid;
//	int	file[2];
//	int	status;
//	// handle cmd execution path. 
//	count = count_cmd(shell->cmd);

//	pipes = NULL;
//	if (count > 1)
//		pipes = create_pipes(shell);

//	file[0] = 0;
//	file[1] = 1;
//	pid = create_process(shell, pipes, file);
//	if (pid == -1)
//	{
//		close_pipes(pipes);
//		shell->exit = 1;
//	}
//	else if (waitpid(pid, &status, 0) > 0)
//	{
//		if (WIFEXITED(status))
//			shell->exit = WEXITSTATUS(status);
//		else if (WIFSIGNALED(status))
//			shell->exit = 128 + WTERMSIG(status);
//	}
//	while (wait(NULL) > 0)
//		;
//	printf("only for use of pid. %d", pid);
//	return ;
//}

///** Question:
// * TODO: jd: signal on child process. kl: reinterragete execusion part. 
//*/

//void	executor_tmp(t_shell *shell)
//{
//	int	status;
//	t_pipe	pipe_params;		// TODO: Put it into the t_shell?

//	pipe_params = (t_pipe){0};
//	pipe_params.cmd_count = count_cmd(shell->cmd);
//	pipes_initializer(&pipe_params);

//	// Now should be the pipe_handler
//	pipe_handler(shell, &pipe_params);

//	//status = wait_handler(&pipe_params);
//	// for pipex program, the main takes (int argc, char **argv, char **envp)
//}
