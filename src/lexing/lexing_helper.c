#include "parse.h" //change to minishell.h after combin
#include "struct.h"
#include "libft.h"
#include <stdio.h>	// for printf
#include <stdlib.h>

void	append_to_lst(t_token **head, t_token *node)
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
	printf("node = %s\n", node->value);
}

/**
 * This function copy the dst string and cat the src string into a new string and free the dst and src, return the new pointer. 
 */
char	*append_to_str(char *dst, char *src)
{
	size_t	dst_len;
	size_t	src_len;
	char	*new;

	if (!src)
		return (NULL);
	dst_len = 0;
	if (dst)
		dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	new = malloc(dst_len + src_len + 1);
	if (!new)
		return (free(src), NULL);
	if (dst)
		ft_strlcpy(new, dst, dst_len + 1);
	else
		new[0] = '\0';
	ft_strlcat(new, src, dst_len + src_len + 1);
	return (free(src), new);
}

void	create_token_node(char *value, t_shell *shell, t_token_type type)
{
	t_token	*node;

	node = (t_token*)malloc(sizeof(t_token) * 1);
	if (!node)
		ft_malloc_failure("Failture at malloc token node.\n", shell);
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(value);
		ft_malloc_failure("Failure at malloc env.\n", shell);
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
size_t	find_index(char *str, size_t end, char c)
{
	size_t	i;
	
	i = 0;
	while (i < end)
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (i);
}
