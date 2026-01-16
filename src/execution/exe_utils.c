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
#include <string.h>
#include <errno.h>

void	find_file_redir(t_cmd *cmd)
{
	t_redir *redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			if (cmd->fd[0] > 0)
				close_fd(&(cmd->fd[0]));
			cmd->fd[0] = redir->fd;
		}
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if (cmd->fd[1]!= 1)
				close_fd(&(cmd->fd[1]));
			cmd->fd[1] = redir->fd;
		}
		redir = redir->next;
	}
	printf("fd0 = %d, fd1 = %d\n", cmd->fd[0], cmd->fd[1]);
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

void	execve_cmd(t_shell *shell, t_cmd *cmd)
{
	char	**env;

	env = env_to_array(shell->env_lst);
	if (!env)
		return (ft_malloc_error("create env for execution", shell));
	if (!cmd->path || cmd->path[0] == '\0')
	{
		ft_execve_error(cmd->cmd[0], "command not found.");
		shell->exit = EXIT_NOCMD;
		free_2d_arr(env);
		return ;
	}
	if (execve(cmd->path, cmd->cmd, env) == -1)
	{
		ft_execve_error(cmd->cmd[0], strerror(errno));
		shell->exit = EXIT_CMD_NOEXC;
	}
	free_2d_arr(env);
}

// TODO: builtin funciton need to set the shell->exit when it fail and need to exit
// builtin function exit(shell->exit); shell->exit is changed when it fails 
// This function has to have the third arguement, t_cmd*, although *shell has cmd, as the third
// arguement specify which command to be executed when there are multiple of them. 
void	execve_builtin(t_shell *shell, int command_type, t_cmd *cmd)
{
	int	status;

	status = 1;
	// Temporary just set exit status to 0 for now
	// shell->exit needs to be set properly after the builtin function is implemented
	// TODO: implement the builtin functions
	if (command_type == 1)
		status = ft_echo(cmd->cmd, shell);
	if (command_type == 2)
		status = ft_cd(cmd->cmd, shell);
	if (command_type == 3)
		status = ft_pwd(cmd->cmd, shell);
	if (command_type == 4)
		status = ft_export(cmd->cmd, shell);
	if (command_type == 5)
		status = ft_unset(cmd->cmd, shell);
	if (command_type == 6)
		status = ft_env(cmd->cmd, shell);
	if (command_type == 7)
		status = ft_exit(cmd->cmd, shell);

	shell->exit = status;
	if (status != 0)
	{
		// something worng happened with running the builtin command
	}
}
