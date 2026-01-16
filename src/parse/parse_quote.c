#include "parse.h" 
#include "struct.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> 

static size_t	next_index(char *str, t_quotok **tok, t_quote *status, bool hd)
{
	size_t		start;
	size_t		end;
	size_t		exp_i;
	char		*value;

	value = NULL;
	start = check_quote(status, str);
	end = find_stop(str, status);
	//printf("%s: start at %zu end at %zu\n", str, start, end);
	if (!hd)
	{
		exp_i = find_index(str, ft_strlen(str), '$');
		if (exp_i < end && *status != SINGLE_QUOTE)
			end = exp_i;
	}
	value = ft_substr(str, start, end - start);
	//printf("after condition check expands and value = %s\n", value);
	if (!value)
		return (-1);
	if (create_quotok_node(value, tok) == -1)
		return (-1);
	//printf("After create node: value = %s\n", value);
	if (*status != GENERAL && (str[end] == '\'' || str[end] == '\"'))
	{
		*status = GENERAL;
		return (free_charptr(&value), ++end);
	}
	return (free_charptr(&value), end);
}

/** Return t_quotok, NULL means malloc fails */
t_quotok	*tokenize_quote(char *value, t_shell *shell, bool hd)
{
	size_t		start;
	size_t		increase;
	t_quotok	*tok;
	t_quote		status;
	size_t		len;

	start = 0;
	tok = NULL;
	status = GENERAL;
	len = ft_strlen(value);
	while (start < len)
	{
		if (!hd && value[start] == '$')
			increase = handle_expands(value + start, shell, &tok);
		else
			increase = next_index(value + start, &tok, &status, hd);
		if (increase == (size_t)-1)
			return (free_quotok(&tok), NULL);
		start += increase;
		//printf("increase = %zu, len = %zu, start = %zu\n", increase, len, start);
	}
	return (tok);
}

char	*join_quotok(t_quotok *quotok)
{
	char	*value;
	char	*tmp;

	value = NULL;
	tmp = NULL;
	//printf("quotok is created\n");
	//print_quotok(quotok);
	value = ft_strdup(quotok->value);
	if (!value)
		return (NULL);
	quotok = quotok->next;
	while (quotok)
	{
		//printf("find next quotok: value = %s\n", quotok->value);
		tmp = value;
		value = ft_strjoin(tmp, quotok->value);
		free_charptr(&tmp);
		if (!value)
			return (NULL);
		quotok = quotok->next;
	}
	//printf("Joined tokens, value = %s\n", value);
	return (value);
}

/**The function process string (parsing value and heredoc's delimiter) which might contain quotes and expands. 
 * It will seperate string into token linked list by quotes and expands.
 * After tokenization, it will join all the linkedlist value into one string.
 */
char	*remove_quote(char *value, t_shell *shell, bool hdoc)
{
	t_quotok *quotok;
	char	*res;

	quotok = NULL;
	res = NULL;
	quotok = tokenize_quote(value, shell, hdoc);
	if (!quotok)
		return (NULL);
	// check somthing to see whether continue or return ;
	//print_quotok(quotok);
	res = join_quotok(quotok);
	free_quotok(&quotok);
	if (!res)
		return (NULL);
	//printf("res=%s\n", res);
	return (res);
}
