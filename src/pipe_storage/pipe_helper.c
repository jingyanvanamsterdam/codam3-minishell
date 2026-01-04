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

/**clsoe all the redir_fd, if exits. othersie close file[0] (=0) and file[1] (=1) */
void	close_files(t_cmd *cmd, int *file)
{
	t_redir *redir;


	// file[2] = [1, 54]
	redir = cmd->redir;
	while (redir)
	{
		if (redir->fd != -1)
			close(redir->fd);
		redir = redir->next;
	}
	if (file[0] == 0)
		close(file[0]);
	if (file[1] == 1)
		close (file[1]);
}
// ==============
void	close_infiles(t_cmd *cmd, int infile)
{
	t_redir *redir;


	// file[2] = [1, 54]
	redir = cmd->redir;
	while (redir)
	{
		if (redir->fd != -1 && (redir->type == REDIR_IN || redir->type == HEREDOC))
			close(redir->fd);
		redir = redir->next;
	}
	if (infile == 0)
		close(infile);
}

void	close_outfiles(t_cmd *cmd, int outfile)
{
	t_redir *redir;


	// file[2] = [1, 54]
	redir = cmd->redir;
	while (redir)
	{
		if (redir->fd != -1 && (redir->type == REDIR_OUT || redir->type == APPEND))
			close(redir->fd);
		redir = redir->next;
	}
	if (outfile == 0)
		close(outfile);
}

/**
 * files are opened; do we need to close everything at once or do it seprately on each fork?
 * 	- if at once, close all the pipes and close all the redir fds
 * 	- if not, close by checking close which pipe and fd.
 * This one now is being called at each fork.
 */
void	parent_close_file(t_shell *shell, int i, int **pipes, int *file, t_cmd *cmd)
{
	int	count;

	count = count_cmd(shell->cmd);
	if (i != 0)
		close(pipes[i - 1][0]);
	else if (file[0] != -1)
		close_infiles(cmd, file[0]);



	if (i < count - 1)
		close(pipes[i][1]);
	else if (file[1] != -1)
		close_outfiles(cmd, file[1]);
}

// int	**pipe; {[, ], [, ], [, ]}
