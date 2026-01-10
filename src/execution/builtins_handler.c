#include "minishell.h"
#include <stdio.h>
#include "libft.h"

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

