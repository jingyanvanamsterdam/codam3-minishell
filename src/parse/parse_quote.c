#include "parse.h" 
#include "struct.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> 

static void	append_to_lst(t_quotok **head, t_quotok *node)
{
	t_quotok	*tmp;

	if (*head == NULL)
	{
		*head = node;
		return ;
	}
	tmp = *head;
	while(tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

int	create_quotok_node(char *value, t_quotok **head)
{
	t_quotok	*node;

	//if (ft_strlen(value) == 0)
	//	return (-1);
	node = (t_quotok*)malloc(sizeof(t_quotok));
	node->value = ft_strdup(value);
	//printf("create tok value = %s\n", node->value);
	if (!node || !node->value)
	{
		free(value);
		free_quotok(head);
		if (node)
			free(node);
		return (-1);
	}
	node->next = NULL;
	append_to_lst(head, node);
	//print_quotok(*head);
	return (1);
}

/** Returned value is the index of a quote or NULL; */
size_t	find_stop(char *str, t_quote *status)
{
	size_t	i;
	
	i = 0;
	if (str[0] == '\'' || str[0] == '\"')
		i++;
	if (*status == SINGLE_QUOTE)
	{
		while (str[i] && str[i] != '\'')
			i++;
		printf("sq: find stop i = %zu\n", i);
	}
	else if (*status == DOUBLE_QUOTE)
	{
		while (str[i] && str[i] != '\"')
			i++;
		printf("dq: find stop i = %zu\n", i);
	}
	else
	{
		while (str[i] && str[i] != '\'' && str[i] != '\"')
			i++;
		printf("find stop i = %zu\n", i);
	}
	return (i);
}
/**
 * change status if the index 0 is a quote
 * The status will change back if the status is not General and the end index is quote. then end + 1 to skip the close quote
 * */
size_t	check_quote(t_quote *status, char *str)
{
	size_t	start;

	start = 0;
	if (str[0] == '\'' || str[0] == '\"')
	{
		start = 1;
		if (str[0] == '\'')
			*status = SINGLE_QUOTE;
		else if(str[0] == '\"')
			*status = DOUBLE_QUOTE;
	}
	return (start);
}

size_t	next_index(char *str, t_quotok **tok, t_quote *status, bool hd)
{
	size_t		start;
	size_t		end;
	size_t		exp_i;
	char		*value;

	value = NULL;
	start = check_quote(status, str);
	end = find_stop(str, status);
	printf("%s: start at %zu end at %zu\n", str, start, end);
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
		return (free(value), ++end);
	}
	return (free(value), end);
}

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
		{
			free_quotok(&tok);
			ft_malloc_failure("at quote token\n", shell);
		}
		start += increase;
		printf("increase = %zu, len = %zu, start = %zu\n", increase, len, start);
	}
	return (tok);
}
