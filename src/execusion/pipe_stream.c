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
#include <signal.h> //for g_sig

volatile sig_atomic_t g_sig = 0;

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

	if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
		ft_error_printing(file);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		perror("open file\n");
	return (fd);
}

int	contain_quote(char *delimiter)
{
	int	i;
	int check;

	i = 0;
	check = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '\'' || delimiter[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

int	heredoc(t_shell *shell, t_redir *redir)
{
	char *input;
	char *res;
	contain_quote(redir->file);
	// readline and make a tmp open file(?);
	res = NULL;
	input = NULL;
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			//ft_warning_printing("minishell: warning: here-document at line %d delimited by end -of-file (wanted \%s')", linenumber, redir->file);
			printf("warning\n");
			break ;
		}
		//Handle input
		/**
		 * if meat delimiter; break;
		 * if contain_quote is false: check for expansion -> input = handle_expansion(input);
		 * else: take input literally.
		 * res = append_to_str(res, input)
		 */
		//if (meet_delimiter(delimiter, input))
		//{
		//	break;
		//}
		//if (!contain_quote(redir->file))
		//{
		//	input = handle_expands(input);
		//}
		//res = append_to_str(res, input);//input is free in the function.
	}
	//write(file[0], tmp, ft_strlen(tmp));
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
