#include "lex.h" //change to minishell.h after combin
#include "struct.h"
#include "libft.h"
#include <stdbool.h> // for boolean functions 
#include <stdio.h>	// for printf
#include <stdlib.h>

/**
 * quote_i == end, means there is no quote
 */
size_t	update_start(char *str, t_shell *shell)
{
	size_t	end;
	size_t	quote_i;
	size_t	expands_i;
	char	*value;

	end = 0;
	value = NULL;
	end = find_end(str);
	if ((quote_i = quote_index(str, end, &(shell->status)))< end)
		end = find_close_quote(str, quote_i, end, shell);
	if ((expands_i = find_index(str, end, '$')) < quote_i)
		value = handle_expands(str, quote_i - expands_i, shell);
	else 
		value = ft_substr(str, 0, quote_i);
	if (!value)
		ft_malloc_failure("Failture at malloc - tokenization\n", shell);
	if (quote_i < end)
		value = append_to_str(value, handle_quote(++str + quote_i, end - quote_i, shell)); 
	if (!value)
		ft_malloc_failure("Failture at malloc - tokenization\n", shell);
	create_token_node(value, shell, WORD);
	free(value);
	return (end);
}

size_t	handle_special_symbol(char *str, size_t end, t_shell *shell)
{
	if (str[end] == '|')
		create_token_node("|", shell, PIPE);
	else if (str[end] == '>')
	{
		if (str[end + 1] && str[end + 1] == '>')
		{
			create_token_node(">>", shell, APPEND);
			end++;
		}
		else
			create_token_node(">", shell, REDIR_OUT);
	}
	else if (str[end] == '<')
	{
		if (str[end + 1] && str[end + 1] == '<')
		{
			create_token_node("<<", shell, HEREDOC);
			end++;
		}
		else
			create_token_node("<", shell, REDIR_IN);
	}
	return (++end);
}

size_t	skip_space_or_quotes(char *str, size_t end)
{
	if (str[end])
	{
		if (str[end] == '\'' || str[end] == '\"')
			end++;
		while(ft_isspace(str[end]))
			end++;
	}
	return (end);
}

t_token *tokenization(char *input, t_shell *shell)
{
	size_t	start;
	size_t	input_len;

	start = 0;
	input_len = ft_strlen(input);
	while (start < input_len)
	{
		start += update_start(input + start, shell);
		if (input[start] == '|' || input[start] == '<' || input[start] == '>')
			start = handle_special_symbol(input, start, shell);
		else
			start = skip_space_or_quotes(input, start);
	}
	return (shell->token);
}
