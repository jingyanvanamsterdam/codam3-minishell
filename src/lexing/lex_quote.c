#include "parse.h" 
#include "libft.h"
#include "struct.h"
#include <stdlib.h>
#include <stdio.h> // for printf

/**
 * change status and return the index of the start quote. 
 */
size_t	quote_index(char *str, size_t end)
{
	size_t i;

	i = 0;
	while (i < end)
	{
		if (str[i] == '\'')
			return (i);
		else if (str[i] == '\"')
			return (i);
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
size_t	find_close_quote(char *str, size_t start, size_t end, t_shell *shell)
{
	if (str[start] == '\'')
	{
		while (str[++start])
			if (str[start] == '\'')
				break ;
	}
	else if (str[start] == '\"')
	{
		while (str[++start])
			if (str[start] == '\"')
				break ;
	}
	if (end < start && str[start])
		return (++start);
	else if (end == start || !str[start])
	{
		ft_input_error("unclosed quote\n", "", shell);
		return ((size_t)-1);
	}
	return (end);
}
