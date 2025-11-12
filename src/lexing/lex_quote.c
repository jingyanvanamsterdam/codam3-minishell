#include "lex.h"
#include "libft.h"
#include "struct.h"
#include <stdlib.h>
#include <stdio.h> // for printf

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
	size_t	i;
	size_t	exp_i;
	size_t	squote_i;

	res = NULL;
	i = 0;
	exp_i = 0;
	if (shell->status == SINGLE_QUOTE)
		return (ft_substr(str, 0, len));
	else if (shell->status == DOUBLE_QUOTE)
	{
		//handle double qutoe;
		squote_i = find_index(str, len, '\'');
		//if (squote_i == len)
	
		//if ((exp_i = find_index(str, len, '$')) == len)
			//handle_expands;

	}
	shell->status = GENERAL;
	return (res);
}