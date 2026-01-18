/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexing.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: kuyu <kuyu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/01/18 15:59:28 by kuyu          #+#    #+#                 */
/*   Updated: 2026/01/18 18:06:43 by jdong         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "struct.h"
#include "libft.h"
#include <stdlib.h>
#include "utils.h"

/**
 * quote_i == end, means there is no quote
 * return -1 means failture; 
 * 1. find_close_quote() return -1 means unclose quote.
 * 2. value used to create token can be malloc failure.
 * 3. create_token_node() malloc failture
 */
size_t	update_start(char *str, t_shell *shell)
{
	size_t	end;
	size_t	quote_i;
	char	*value;

	end = 0;
	value = NULL;
	end = find_end(str);
	quote_i = quote_index(str, end);
	if (quote_i < end)
		end = find_close_quote(str, quote_i, end, shell);
	if (end == (size_t) - 1)
		return ((size_t) - 1);
	value = ft_substr(str, 0, end);
	if (!value)
		return (ft_malloc_error("lexing", shell), (size_t) - 1);
	if (!create_token_node(value, shell, WORD))
		return (free_charptr(&value), (size_t) - 1);
	free_charptr(&value);
	return (end);
}

static int	check_next_symbol(int s, t_shell *shell, char *str, size_t *end)
{
	int	ret;

	ret = 0;
	if (s == 1)
	{
		if (str[(*end) + 1] && str[(*end) + 1] == '>')
		{
			ret = create_token_node(">>", shell, APPEND);
			(*end)++;
		}
		else
			ret = create_token_node(">", shell, REDIR_OUT);
	}
	else if (s == 0)
	{
		if (str[(*end) + 1] && str[(*end) + 1] == '<')
		{
			ret = create_token_node("<<", shell, HEREDOC);
			(*end)++;
		}
		else
			ret = create_token_node("<", shell, REDIR_IN);
	}
	return (ret);
}

/**return -1 if create_token_node() malloc failed
 * return next index after the symbol;
 */
size_t	handle_special_symbol(char *str, size_t end, t_shell *shell)
{
	int	ret;

	ret = -1;
	if (str[end] == '|')
		ret = create_token_node("|", shell, PIPE);
	else if (str[end] == '>')
		ret = check_next_symbol(1, shell, str, &end);
	else if (str[end] == '<')
		ret = check_next_symbol(0, shell, str, &end);
	if (!ret)
		return ((size_t) - 1);
	return (++end);
}

size_t	skip_space(char *str, size_t end)
{
	if (str[end])
		while (ft_isspace(str[end]))
			end++;
	return (end);
}

int	tokenization(t_shell *shell)
{
	size_t	start;
	size_t	input_len;
	size_t	increase;
	char	*input;

	start = 0;
	input = shell->input;
	input_len = ft_strlen(input);
	while (start < input_len)
	{
		increase = update_start(input + start, shell);
		if (increase == (size_t) - 1)
			return (0);
		start += increase;
		if (input[start] && (input[start] == '|'
				|| input[start] == '<' || input[start] == '>'))
			start = handle_special_symbol(input, start, shell);
		if (start == (size_t) - 1)
			return (0);
		if (input[start])
			while (ft_isspace(input[start]))
				start++;
	}
	return (1);
}
