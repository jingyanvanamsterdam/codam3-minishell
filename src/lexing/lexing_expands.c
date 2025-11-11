#include "lex.h"
#include <stdbool.h> // for boolean functions
#include "struct.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> // for printf

bool	contain_expands(char *str, int end)
{
	while (end >= 0)
	{
		if (str[end] == '$')
			return (true);
		end--;
	}
	return (false);
}

/**
 * return the pointer of a value that contains all the expansion without space.
 */
char	*check_expands(char *str, int len, t_shell *shell)
{
	int		i;
	int		start;
	char	*res;

	i = 0;
	res = NULL;
	start = 0;
	while (i < len)
	{
		if (str[i] == '$')
		{
			res = join_prev_expands(str, start, i, shell, res);
			if (!res)
				return (NULL);
			i += skip_expansion(str);
			start = i;
		}
		else
			i++;
	}
	if (start < i)
		res = append_to_str(res, ft_substr(str, start, i - start));
	if (!res)
		return (NULL);
	return (res);
}
