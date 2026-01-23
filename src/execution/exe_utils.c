/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exe_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: kuyu <kuyu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/01/18 15:39:22 by kuyu          #+#    #+#                 */
/*   Updated: 2026/01/23 16:24:03 by jdong         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>		// for EXIT_FAILURE
#include <sys/wait.h>
#include <unistd.h>		// for STDIN_FILENO
#include <string.h>
#include <errno.h>

/** if no redir, cmd_fd default 0 en 1; 
 * if there are several same type of redir previous will close and 
 * neww will be open
 * if open fails because of no permission or no such file, return 0
 * in that case, no dup or execve will run.*/
int	open_files(t_cmd *cmd, t_redir *redir, t_type type)
{
	if (cmd->fd[0] > 0 && cmd->fd[0] != cmd->hdfd
		&& (type == REDIR_IN || type == HEREDOC))
		close_fd(&(cmd->fd[0]));
	if (type == REDIR_IN)
		cmd->fd[0] = open(redir->file, O_RDONLY);
	else if (type == HEREDOC)
		cmd->fd[0] = cmd->hdfd;
	if (cmd->fd[0] == -1)
		return (ft_error_printing(redir->file), 0);
	if (cmd->fd[1] > 1 && (type == REDIR_OUT || type == APPEND))
		close_fd(&(cmd->fd[1]));
	if (type == REDIR_OUT)
		cmd->fd[1] = open_outfile(redir->file);
	else if (type == APPEND)
		cmd->fd[1] = output_append(redir->file);
	if (cmd->fd[1] == -1)
		return (ft_error_printing(redir->file), 0);
	return (1);
}

t_builtin	is_builtin(char *command)
{
	if (ft_strcmp("echo", command) == 0)
		return (ECHO);
	if (ft_strcmp("cd", command) == 0)
		return (CD);
	if (ft_strcmp("pwd", command) == 0)
		return (PWD);
	if (ft_strcmp("export", command) == 0)
		return (EXPORT);
	if (ft_strcmp("unset", command) == 0)
		return (UNSET);
	if (ft_strcmp("env", command) == 0)
		return (ENV);
	if (ft_strcmp("exit", command) == 0)
		return (EXIT);
	return (OTHERS);
}

static void	ft_handle_execve_err(char *cmd, t_shell *shell)
{
	if (errno == ENOENT)
	{
		ft_execve_error(cmd, "command not found");
		shell->exit = EXIT_NOCMD;
	}
	else
	{
		shell->exit = EXIT_CMD_NOEXC;
		if (errno == EACCES)
			ft_execve_error(cmd, "permission denied");
		else if (errno == EISDIR)
			ft_execve_error(cmd, "is a directory");
		else
			ft_execve_error(cmd, strerror(errno));
	}
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
		ft_handle_execve_err(cmd->cmd[0], shell);
	free_2d_arr(env);
}

// builtin funciton need to set the shell->exit when it fail and need to exit
// builtin function exit(shell->exit); shell->exit is changed when it fails 
// This function has to have the arguement, t_cmd*, although *shell has cmd, 
// as this arguement specify which command to execute 
// when there are multiple of them. 
void	execve_builtin(t_shell *shell, t_builtin type, t_cmd *cmd)
{
	int	status;

	status = 0;
	if (type == ECHO)
		status = ft_echo(cmd->cmd, shell);
	if (type == CD)
		status = ft_cd(cmd->cmd, shell);
	if (type == PWD)
		status = ft_pwd(cmd->cmd, shell);
	if (type == EXPORT)
		status = ft_export(cmd->cmd, shell);
	if (type == UNSET)
		status = ft_unset(cmd->cmd, shell);
	if (type == ENV)
		status = ft_env(cmd->cmd, shell);
	if (type == EXIT)
		status = ft_exit(cmd->cmd, shell);
	shell->exit = status;
}
