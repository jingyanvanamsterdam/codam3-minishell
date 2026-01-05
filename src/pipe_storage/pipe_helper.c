#include "struct.h"
// #include "pipe.h"
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

///**
// * files are opened; do we need to close everything at once or do it seprately on each fork?
// * 	- if at once, close all the pipes and close file[0] and file[1]
// * 	- if not, close by checking close which pipe and fd.
// * This one now is being called at each fork.
// */
//void	parent_close_file(t_shell *shell, int i, int **pipes, int *fd)
//{
//	int	count;

//	count = count_cmd(shell->cmd);
//	if (i != 0)
//		close(pipes[i - 1][0]);
//	else if (fd[0] != -1)
//		close(fd[0]);
//	if (i < count - 1)
//		close(pipes[i][1]);
//	else if (fd[1] != -1)
//		close(fd[1]);
//}

// int	**pipe; {[, ], [, ], [, ]}
