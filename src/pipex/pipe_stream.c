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

//char	*handle_delimiter(char *file, t_shell *shell, bool quoted)
//{
//	size_t		i;
//	size_t		quote_end;
//	char		*delimiter;

//	i = 0;
//	quote_end = 0;
//	delimiter = NULL;
//	if (quoted)
//	{
//		while (i < ft_strlen(file))
//		{
//			quote_end = remove_quote(file, &delimiter, shell, false);
//			if (quote_end == -1)
//				ft_malloc_failure("heredoc\n", shell);
//			i = quote_end + 1 + i;
//		}
//	}
//	else
//		delimiter = ft_strdup(file);	
//	if (!delimiter)
//		ft_malloc_failure("heredoc\n", shell);
//}

bool	contain_quote(char *file)
{
	int	i;

	i = 0;
	while (file[i])
	{
		if (file[i] == '\'' || file[i] == '\"')
			return (true);
		i++;
	}
	return (false);
}

void	heredoc(t_shell *shell, t_redir *redir, int readin)
{
	//char	*input;
	//char	*res;
	bool	quoted;
	char	*delimiter;

	//flag = contain_quote(redir->file, delimiter, shell);
	// readline and make a tmp open file(?);
	//res = NULL;
	//input = NULL;
	quoted = contain_quote(redir->file);
	delimiter = remove_quote(redir->file, shell, true);
	if (!delimiter)
		ft_malloc_failure("heredoc\n", shell);
	if (quoted)
		printf("quoted, delimiter = %s\n", delimiter);
	else
		printf("no quote, delimiter = %s\n", delimiter);
	write(readin, "", 1);
	//while (1)
	//{
	//	input = readline("> ");
	//	if (!input)
	//	{
	//		//ft_warning_printing("minishell: warning: here-document at line %d delimited by end -of-file (wanted \%s')", linenumber, redir->file);
	//		printf("warning\n");
	//		break ;
	//	}
	//	//Handle input
	//	/**
	//	 * if meat delimiter; break;
	//	 * if contain_quote is false: check for expansion -> input = handle_expansion(input);
	//	 * else: take input literally.
	//	 * res = append_to_str(res, input)
	//	 */
	//	//if (meet_delimiter(delimiter, input))
	//	//{
	//	//	break;
	//	//}
	//	//if (!contain_quote(redir->file))
	//	//{
	//	//	input = handle_expands(input);
	//	//}
	//	//res = append_to_str(res, input);//input is free in the function.
	//}
	//printf("%s\n", redir->file);
	//write(1, shell->cmd->cmd[0], 3);
	//write(readin, res, ft_strlen(res));
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
