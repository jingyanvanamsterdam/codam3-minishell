// #include "pipe.h" // change to minishell
#include "parse.h"
#include "struct.h"
#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int	dup_files(t_shell *shell, t_cmd *cmd, int stream[2])
{
	int	count;
	int	**pipes;

	count = shell->pip_param->cmd_count;
	pipes = shell->pip_param->pipes;
	if (dup2(stream[0], STDIN_FILENO) < 0)
	{
		shell->exit = 1;
		ft_error_printing("dup2 input fail");
	}
	if (dup2(stream[1], STDOUT_FILENO) < 0)
	{
		shell->exit = 1;
		ft_error_printing("dup2 output fail");
	}
	close_pipes(shell->pip_param);
	if (cmd->fd[0] != -1)
		close(cmd->fd[0]);
	if (cmd->fd[1] != -1)
		close(cmd->fd[1]);
	return (shell->exit);
}

void	execve_cmd(t_shell *shell, t_cmd *cmd)
{
	int 	j;
	//char	**enviorn;

	j = 0;
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
	if (execve(cmd->path, cmd->cmd, env_to_array(shell->env_lst)) == -1)
	{
		ft_error_printing(cmd->cmd[0]);
		shell->exit = EXIT_CMD_NOEXC;
	}
}

void	execve_builtin(t_shell *shell, int command_type, t_cmd *cmd)
{
	if (command_type == 1)
		ft_echo(cmd->cmd, shell);
	if (command_type == 2)
		ft_cd(cmd->cmd, shell);
	if (command_type == 3)
		ft_pwd(cmd->cmd, shell);
	if (command_type == 4)
		ft_export(cmd->cmd, shell);
	if (command_type == 5)
		ft_unset(cmd->cmd, shell);
	if (command_type == 6)
		ft_env(cmd->cmd, shell);
	if (command_type == 7)
		ft_exit(cmd->cmd, shell);
}

int	is_builtin(char *command)
{
	if (ft_strcmp("echo", command) == 0)
		return (1);
	if (ft_strcmp("cd", command) == 0)
		return (2);
	if (ft_strcmp("pwd", command) == 0)
		return (3);
	if (ft_strcmp("export", command) == 0)
		return (4);
	if (ft_strcmp("unset", command) == 0)
		return (5);
	if (ft_strcmp("env", command) == 0)
		return (6);
	if (ft_strcmp("exit", command) == 0)
		return (7);
	return (0);
}

void	run_child_process(t_shell *shell, t_cmd *cmd, int stream[2])
{
	int	command;
	int	**pipes;

	setup_child_signal();
	pipes = shell->pip_param->pipes;
	if (cmd->cmd)
	{
		command = is_builtin(cmd->cmd[0]);
		if (dup_files(shell, cmd, stream) == 0)
		{
			if (command > 0)
				execve_builtin(shell, cmd, command);
			else
				execve_cmd(shell, cmd);
		}
	}
	free_pipes(shell->pip_param);
	exit(shell->exit);
}

//================================================================
// Before fork process: 

/**
 * files are opened; do we need to close everything at once or do it seprately on each fork?
 * 	- if at once, close all the pipes and close file[0] and file[1]
 * 	- if not, close by checking close which pipe and fd.
 * This one now is being called at each fork.
 */
void	parent_close_file(t_shell *shell, int i)
{
	int	count;
	int **pipes;

	count = shell->pip_param->cmd_count;
	pipes = shell->pip_param->pipes;
	if (i != 0)
		close(pipes[i - 1][0]);
	else if (shell->cmd->fd[0] != -1)
		close(shell->cmd->fd[0]);
	if (i < count - 1)
		close(pipes[i][1]);
	else if (shell->cmd->fd[1] != -1)
		close(shell->cmd->fd[1]);
}

/** The function close the previous redirect fd if there is any. */
void	find_file_redir(t_cmd *cmd)
{
	t_redir *redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			if (cmd->fd[0] > 0)
				close(cmd->fd[0]);
			cmd->fd[0] = redir->fd;
		}
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if (cmd->fd[1] != -1 && cmd->fd[1]!= 1)
				close(cmd->fd[1]);
			cmd->fd[1] = redir->fd;
		}
		redir = redir->next;
	}
}

void	setup_stream(int stream[2], t_cmd *cmd, int i, t_shell *shell)
{
	int	count;
	int	**pipes;

	count = shell->pip_param->cmd_count;
	pipes = shell->pip_param->pipes;
	if (i != 0 && cmd->fd[0] == 0)
		stream[0] = pipes[i - 1][0];
	else 
		stream[0] = cmd->fd[0];
	if (i == (count - 1) && cmd->fd[1] == 1)
		stream[1] = pipes[i][1];
	else
		stream[1] = cmd->fd[1];
}
/** find file redir() is change in/out fd if there is redirection in each cmd
 * After that set pipes when there is no redirection on the comd (aka fd == initial value 0 and 1)
 * parent_close_file() not sure do it seperately like now or all at once after while.
 * run_child_process() should be reintergrate with KL code.
*/
int	create_process(t_shell *shell)
{
	int		i;
	pid_t	pid;
	int		count;
	t_cmd	*cmd;
	int		stream[2];

	i = -1;
	count = count_cmd(shell->cmd);
	cmd = shell->cmd;
	while (++i < count)
	{
		find_file_redir(cmd);
		setup_stream(stream, cmd, i, shell);
		pid = fork();
		if (pid < 0)
			return (perror("sh: fork"), -1);
		else if (pid == 0)
			run_child_process(shell, cmd, stream);
		else
			parent_close_file(shell, i);
		cmd = cmd->next;
	}
	return (pid);
}
