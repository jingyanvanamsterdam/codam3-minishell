#include "parse.h" //change to minishell.h after combin
#include "struct.h"
#include "libft.h"
#include <stdio.h>	// for printf
#include <stdlib.h>
#include "utils.h"


static void	append_to_lst(t_token **head, t_token *node)
{
	t_token	*tmp;

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

/**
 * check if value = "", not create token node.
 * ft_substr might create empty string if it is only a stop symbol < > | 
 */
void	create_token_node(char *value, t_shell *shell, t_type type)
{
	t_token	*node;

	if (ft_strlen(value) == 0)
		return ;
	node = (t_token*)malloc(sizeof(t_token));
	if (!node)
		ft_malloc_failure("tokenization.\n", shell);
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(value);
		free(node);
		ft_malloc_failure("tokenization.\n", shell);
	}
	node->type = type;
	node->next = NULL;
	append_to_lst(&(shell->token), node);
}

/**
 * This func return the index of the end of a string/
 * Based on status, the end requirement is different.
 */
size_t	find_end(char *str)
{
	size_t	end;
	
	end = 0;
	while (str[end] && !ft_isspace(str[end]) 
		&& str[end] != '|' && str[end] != '<' && str[end] != '>')
		end++;
	return (end);
}

/**
 * Return the index of the charater, if there is no, it will equal to the end.
 * To do: Maybe replace with ft_strchr(), but some compare logic need to be changed.
 */
size_t	find_index(char *str, size_t len, char c)
{
	size_t	i;
	
	i = 0;
	while (i < len)
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (i);
}
