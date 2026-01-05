#include "pipe.h"

// shell->cmd ======> ? =====> char **argv
// shell->env_lst ======> ? =====> char **envp
// child process takes the 

void	child_process(char **argv, int i, char **envp, t_pipe *params)
{
	t_child	child;

	/* 
	t_child has *path, and a **argv;
	*/

	child = (t_child){0};
	fd_preparation(argv, i, params);
	child.argv = ft_split(argv[i + 2], ' ');
	if (!child.argv || !child.argv[0] || child.argv[0][0] == '\0')
	{
		ft_putstr_fd(" : command not found \n", 2);
		child_process_exiter(&child, params, 127);
	}
	child.path = find_path(child.argv[0], envp);
	if (!child.path)
	{
		ft_putstr_fd(child.argv[0], 2);
		ft_putstr_fd(" : command not found \n", 2);
		child_process_exiter(&child, params, 127);
	}
	execve(child.path, child.argv, envp);
	perror(child.path);
	child_process_exiter(&child, params, EXIT_FAILURE);
}
