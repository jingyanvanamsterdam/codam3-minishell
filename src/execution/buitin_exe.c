#include "minishell.h"
#include <fcntl.h>		// for pid_t
#include <stdlib.h>		// for EXIT_FAILURE
#include <sys/wait.h>
#include <unistd.h>		// for STDIN_FILENO
#include <stdio.h>		// for perror()
#include "libft.h"
#include <errno.h>

// static int  open_files(t_shell *shell)
// {
//     t_cmd   *cmd;

//     cmd = shell->cmd;
//     if (cmd->intype == REDIR_IN && cmd->infile)
//     {
//         cmd->fd[0] = open(cmd->infile, O_RDONLY);
//         if (cmd->hdfd != -1)
//             close_fd(&(cmd->hdfd));
//         if (cmd->fd[0] == -1)
//             return (ft_error_printing(cmd->infile), 0);
//     }
//     else if (cmd->intype == HEREDOC)
//         cmd->fd[0] = cmd->hdfd;
//     if (cmd->outtype == REDIR_OUT && cmd->outfile)
//         cmd->fd[1] = open_outfile(cmd->outfile);
//     else if (cmd->outtype == APPEND && cmd->outfile)
//         cmd->fd[1] = output_append(cmd->outfile);
//     if (cmd->fd[1] == -1)
//         return (ft_error_printing(cmd->outfile), 0);
//     return (1);
// }

// Return shell->exit; 0 for success, 1 for error (shell continues)
// close_fd will close and change it to -1;
// This function return 1 when error occurs.
int	apply_redir_parent(t_shell *shell, int savefd[2])
{
	if (shell->cmd->fd[0] != 0)
	{
		savefd[0] = dup(STDIN_FILENO);
		if (dup2(shell->cmd->fd[0], STDIN_FILENO) < 0)
		{
			shell->exit = 1;
			ft_error_printing("fail at dup2");
		}
	}
	if (shell->cmd->fd[1] != 1)
	{
		savefd[1] = dup(STDOUT_FILENO);
		if (dup2(shell->cmd->fd[1], STDOUT_FILENO) < 0)
		{
			shell->exit = 1;
			ft_error_printing("fail at dup2");
		}
	}
    close_cmd_fds(shell);
	return (shell->exit);
}

void	restore_parent_fd(int saved_stdfd[2])
{
	if (saved_stdfd[0] != -1)
	{
		dup2(saved_stdfd[0], STDIN_FILENO);
		close(saved_stdfd[0]);
	}
	if (saved_stdfd[1] != -1)
	{
		dup2(saved_stdfd[1], STDOUT_FILENO);
		close(saved_stdfd[1]);
	}
}

// This function return 1 when the command is a single builtin command
// After calling this function, need to check the shell->exit to see if it is 1 or 0.
void	single_builtin_handler(t_shell *shell, t_builtin type)
{
	int		saved_stdfd[2];
    t_redir *redir;

	saved_stdfd[0] = -1;
	saved_stdfd[1] = -1;
    redir = shell->cmd->redir;
    while (redir)
    {
        if (!open_files(shell->cmd, redir, redir->type))
        {
            close_cmd_fds(shell);
            shell->exit = 1;
            return ;
        }
        redir = redir->next;
    }
    if (apply_redir_parent(shell, saved_stdfd) == 0)
        execve_builtin(shell, type, shell->cmd);
    restore_parent_fd(saved_stdfd);
}
