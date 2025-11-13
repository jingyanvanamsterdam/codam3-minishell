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
		end = find_close_quote(str + quote_i, end, shell);
	if ((expands_i = find_index(str, end, '$')) < quote_i)
		value = handle_expands(str, expands_i, quote_i - expands_i, shell);
	else 
		value = ft_substr(str, 0, quote_i);
	printf("bf: end = %zu, qi = %zu, ei = %zu\n", end, quote_i, expands_i);
	if (!value)
		ft_malloc_failure("Failture at malloc - tokenization\n", shell);
	if (quote_i < end)
		value = append_to_str(value, handle_quote(str + quote_i, quote_i - end, shell)); 
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
		// should be space, or \" or \'
		while(ft_isspace(str[end]))
			end++;
		if (str[end] == '\'' || str[end] == '\"')
			while(ft_isspace(str[++end]))
				end++;
	}
	return (end);
}

t_token *tokenization(char *input, t_shell *shell)
{
	size_t	start;

	start = 0;
	size_t	input_len = ft_strlen(input);
	while (start < input_len)
	{
		start += update_start(input + start, shell);
		printf("after update start %zu\n", start);
		if (input[start] == '|' || input[start] == '<' || input[start] == '>')
			start = handle_special_symbol(input, start, shell);
		else
			start = skip_space_or_quotes(input, start);
		printf("after handle symbol start = %zu\n", start);
	}
	return (shell->token);
}

/**
 * ./lex 'echo $abc.txt + 5 "touch $def+$def|$abc<<457" ''
 * sig fault
 * $ expands variable is not being found ==> look up 
 * double quote has logic problem, "touch" is being created as one node. and then got sig fault.
 */