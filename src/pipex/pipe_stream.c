#include "pipe.h" // change to minishell
#include "parse.h"
#include "struct.h"
#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> //for g_sig
#include <stdbool.h>

int	open_infile(char *file)
{
	int	fd;

	if (access(file, F_OK) == -1 || access(file, R_OK) == -1)
	{
		ft_error_printing(file);
		fd = open("/dev/null", O_RDONLY);
	}
	else
		fd = open(file, O_RDONLY);
	if (fd == -1)
		perror("open file\n");
	return (fd);
}

int	open_outfile(char *file)
{
	int fd;

	fd = -1;
	if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
		ft_error_printing(file);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		perror("open file\n");
	return (fd);
}

void	output_append(t_shell *shell, t_redir *redir, int readout)
{
	write(1, shell->cmd->cmd[0], 3);
	write(readout, redir->file, ft_strlen(redir->file));
}

void	redir_file(t_shell *shell, int *file, int i)
{
	t_redir	*redir;
	t_cmd	*cmd;
	int		j;

	j = 0;
	cmd = shell->cmd;
	while (j++ < i)
		cmd = cmd->next;
	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			file[0] = open_infile(redir->file);
		else if (redir->type == REDIR_OUT)
			file[1] = open_outfile(redir->file);
		else if (redir->type == APPEND)
			output_append(shell, redir, file[1]);
		else if (redir->type == HEREDOC)
			heredoc(shell, redir, file[0]);
		redir = redir->next;
	}
}

