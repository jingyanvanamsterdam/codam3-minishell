#include "pipe.h" // change to minishell
#include "struct.h"
#include "libft.h"
#include <stdio.h>	
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int	open_infile(char *file)
{
	int	fd;

	if (access(file, F_OK) == -1 || access(file, R_OK) == -1)
	{
		ft_error_print(file);
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

	if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
		ft_error_printf(file);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		perror("open file\n");
	return (fd);
}

void contain_quote(char *limiter)
{
	int	i;

	i = 0;
	if (limiter[0] == '\'' || limiter[0] == '\"')
	{
		while (limiter[i])
		{
			if (limiter[i] == '\'' || limiter[i] == '\"')
			{
				limiter[i] = '\0';
				break ;
			}
			i++;
		}
	}
}

int	heredoc(t_shell *shell, t_redir *redir)
{
	contain_quote(redir->file);
	// readline and make a tmp open file(?);
}

void	handle_redir(t_shell *shell, int *file)
{
	t_redir	*redir;

	while (shell->cmd)
	{
		redir = shell->cmd->redir;
		while (redir)
		{
			if (redir->type == REDIR_IN)
				file[0] = open_infile(redir->file);
			else if (redir->type == REDIR_OUT)
				file[1] = open_outfile(redir->file);
			else if (redir->type == APPEND)
				file[1] = output_append(shell, redir);
			else if (redir->type == HEREDOC)
				file[0] = heredoc(shell, redir);
			redir = redir->next;
		}
	}
}
