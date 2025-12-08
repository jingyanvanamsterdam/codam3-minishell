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

static size_t	update_index(char *input, t_quotok **tok)
{
	size_t	i;
	size_t	end;
	size_t	exp_i;
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

static char	*handle_exp_input(char *input, t_shell *shell, size_t len)
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
	value = joint_quotok(tok);
	free_quotok(&tok);
	if (!value)
		return (NULL);
	return (value);
}

static char	*handle_hd_input(bool quoted, char *input, char *res, t_shell *shell)
{
	char	new_res;
	int		exp_i;
	int		len;

	new_res = NULL;
	exp_i = 0;
	len = ft_strlen(input);
	if (!quoted)
		exp_i = find_index(input, len, '$');
	if (quoted || exp_i == len)
		new_res = ft_strjoin(res, input);
	else
		new_res = handle_exp_input(input, shell, len);
	if (!new_res)
		return (NULL);
	return (new_res);
}

static char	*do_hd_loop(bool quoted, char *delimiter, t_shell *shell)
{
	char	*input;
	char	*res;
	char 	*tmp;

	input = NULL;
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
		free(tmp);
		free(input);
		if (!res)
			return (NULL);
	}
	return (res);
}

void	heredoc(t_shell *shell, t_redir *redir, int readin)
{
	char	*res;
	bool	quoted;
	char	*delimiter;
	int		i;

	res = NULL;
	quoted = false;
	i = 0;
	while (redir->file[i])
	{
		if (redir->file[i] == '\'' || redir->file[i] == '\"')
			quoted = true;
		i++;
	}
	delimiter = remove_quote(redir->file, shell, true);
	if (!delimiter)
		ft_malloc_failure("heredoc\n", shell);
	res = do_hd_loop(quoted, delimiter, shell);
	if (!res)
		ft_malloc_failure("heredoc\n", shell);
	free(delimiter);
	write(readin, res, ft_strlen(res));
}
