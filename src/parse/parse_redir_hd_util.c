# define _GNU_SOURCE
#include "parse.h"
#include "struct.h"
#include "libft.h"
#include <stdio.h> // printf
#include <readline/readline.h>
#include <readline/history.h>

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
 */
char	*do_hd_loop(bool quoted, char *delimiter, t_shell *shell)
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
		free(input);
		if (!res)
			return (NULL);
	}
	return (res);
}
