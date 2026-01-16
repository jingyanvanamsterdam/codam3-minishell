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
	char	*new;
	char	*tmp;
	int		exp_i;
	int		len;

	exp_i = 0;
	new = res;
	len = ft_strlen(input);
	if (!quoted)
		exp_i = find_index(input, len, '$');
	if (quoted || exp_i == len)
		new = ft_strjoin(res, input);
	else
		new = handle_exp_input(input, shell, len, res);
	if (!new)
		return (NULL);
	tmp = new;
	new = ft_strjoin(tmp, "\n");
	free_charptr(&tmp);
	return (new);
}

static char *heredoc_input(char	*res, bool quoted, char *delimiter, t_shell *shell)
{
	char	*tmp;
	char	*input;

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
		free_charptr(&tmp);
		if (!res)
			return (ft_error_printing("issues at heredoc"), NULL);
	}
	free_charptr(&input);
	return (res);
}
/**
 * tmp is previous result, res is updated by handle_hd_input() and tmp is being freed inside;
 * if ctrl + d end of the heredoc, whatever res before had will be write into the heredoc fd.
 */

void	run_hd_process(bool q, char *deli, t_shell *shell, t_cmd *cmd)
{
	char	*res;

	sig_heredoc();
	res = ft_strdup("");
	if (!res)
		return (ft_malloc_error("heredoc", shell));
	res = heredoc_input(res, q, deli, shell);
	if (!res)
		return ;
	write(cmd->hdfd, res, ft_strlen(res));
	free_charptr(&res);
}
