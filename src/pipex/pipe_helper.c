#include "struct.h"
#include "pipe.h"
#include "parse.h"
#include "libft.h"
#include <unistd.h> //close()

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

void	redirect_fd(int *file, int **pipes, int i, t_shell *shell)
{
	int	count;

	count = count_cmd(shell->cmd);
	if (i == 0)
		file[0] = 0;
	else
		file[0] = pipes[i - 1][0];
	if (i == count - 1)
		file[1] = 1;
	else
		file[1] = pipes[i][1];
}

void	close_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	close_files(int *file)
{
	if (file[0] != -1)
		close(file[0]);
	if (file[1] != -1)
		close (file[1]);
}

// file is not being opened, only opened in the child process. do I need to close them in the parent process?
void	parent_close_file(t_shell *shell, int i, int **pipes, int *file)
{
	int	count;

	count = count_cmd(shell->cmd);
	if (i != 0)
		close(pipes[i - 1][0]);
	else if (file[0] != -1)
		close(file[0]);
	if (i < count - 1)
		close(pipes[i][1]);
	else if (file[1] != -1)
		close(file[1]);
}