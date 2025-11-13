#include "lex.h"
#include "libft.h"
#include "struct.h"
#include <stdlib.h>
#include <stdio.h> // for printf

/**
 * change status and return the index of the start quote. 
 */
size_t	quote_index(char *str, size_t end, t_lex_status *status)
{
	size_t i;

	i = 0;
	while (i < end)
	{
		if (str[i] == '\'')
		{
			*status = SINGLE_QUOTE;
			return (i);
		}
		else if (str[i] == '\"')
		{
			*status = DOUBLE_QUOTE;
			return (i);
		}
		i++;
	}
	return (i);
}

/**
 * Check whether there is close quote. Return the end index.
 * if result i is later than end, return the close quote index.
 * if it is at the same index, meaning it is the end of the string and there is no close quote. 
 * if it is smaller, then end index not change. 
 */
size_t	find_close_quote(char *str, size_t end, t_shell *shell)
{
	size_t	i;

	i = 0;
	if (str[i] == '\'')
	{
		while (str[++i])
			if (str[i] == '\'')
				break ;
	}
	else if (str[i] == '\"')
	{
		while (str[++i])
			if (str[i] == '\"')
				break ;
	}
	if (end < i)
		return (i);
	if (end == i)
		ft_input_error("unclosed quote\n", shell);
	return (end);
}

char	*handle_quote(char *str, size_t len, t_shell *shell)
{
	char	*res;
	size_t	exp_i;

	res = NULL;
	exp_i = find_index(str, len, '$');
	if (shell->status == SINGLE_QUOTE || exp_i == len)
		res = ft_substr(str, 0, len);
	else if (shell->status == DOUBLE_QUOTE && exp_i < len)
		res = handle_expands(str, exp_i, len - exp_i, shell);
	if (!res)
		return (NULL);
	shell->status = GENERAL;
	return (res);
}
