# define _GNU_SOURCE
#include "parse.h"
#include "utils.h"
#include "struct.h"
#include "libft.h"
#include <stdio.h> // printf
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>

static size_t	update_index(char *input, t_quotok **tok)
{
	size_t	i;
	char	*value;

	value = NULL;
	i = 0;
	while (input[i] && input[i] != '$')
		i++;
	value = ft_substr(input, 0, i);
	if (!value)
		return (-1);
	if (create_quotok_node(value, tok) == -1)
		return (-1);
	return (free(value), i);
}

static char	*handle_exp_input(char *input, t_shell *shell, size_t len, char *res)
{
	size_t	start;
	size_t	increase;
	t_quotok	*tok;
	char	*value;

	start = 0;
	tok = NULL;
	value = NULL;
	while (start < len)
	{
		if (input[start] == '$')
			increase = handle_expands(input + start, shell, &tok);
		else
			increase = update_index(input + start, &tok);
		if (increase == (size_t)-1)
			return (free_quotok(&tok), NULL);
		start += increase;
	}
	value = join_quotok(tok);
	free_quotok(&tok);
	if (!value)
		return (NULL);
	return (ft_strjoin(res, value));
}

static char	*handle_hd_input(bool quoted, char *input, char *res, t_shell *shell)
{
	char	*tmp;
	int		exp_i;
	int		len;

	exp_i = 0;
	tmp = res;
	len = ft_strlen(input);
	if (!quoted)
		exp_i = find_index(input, len, '$');
	if (!res)
	{
		tmp = ft_strdup("");
		if (!tmp)
			return (NULL);
	}
	if (quoted || exp_i == len)
		res = ft_strjoin(tmp, input);
	else
		res = handle_exp_input(input, shell, len, tmp);
	free(tmp);
	tmp = res;
	res = ft_strjoin(tmp, "\n");
	free(tmp);
	return (res);
}
/**
 * tmp is previous result, res is updated by handle_hd_input() and tmp is being freed inside;
 * if ctrl + d end of the heredoc, whatever res before had will be write into the heredoc fd.
 */

void	run_heredoc_process(bool quoted, char *delimiter, t_shell *shell, t_redir *redir)
{
	char	*input;
	char	*res;
	char 	*tmp;

	res = NULL;
	tmp = NULL;
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			ft_warning_printing();
			break ;
		}
		if (!ft_strcmp(delimiter, input))
			break ;
		tmp = res;
		res = handle_hd_input(quoted, input, tmp, shell);
		free_charptr(&input);
		if (!res)
			return (ft_error_printing("issues at heredoc"));
	}
	write(redir->fd, res, ft_strlen(res));
	free_charptr(&input);
}

int	heredoc_wait(t_shell *shell, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		shell->exit = 130;
		//printf("heredoc sig int\n");
		return (0);
	}
	//printf("heredoc wait finished, %d\n", WEXITSTATUS(status));
	return (1);	
}
/** return 0 if there is error and fails */
int	do_hd_loop(bool quoted, char *delimiter, t_shell *shell, t_redir *redir)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (ft_pipe_error(shell, "fork", 0), 0);
	else if (pid == 0)
	{
		sig_heredoc();
		redir->fd = open(redir->file, O_WRONLY|O_CREAT|O_EXCL|O_TRUNC, 0600);
		if (redir->fd == -1)
			ft_error_printing("open heredoc file");
		else
			run_heredoc_process(quoted, delimiter, shell, redir);
		close_cmd_fds(shell);
		ft_process_exit(shell, false);
		return (1);
	}
	else
		return (heredoc_wait(shell, pid));
}
