/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexing_helper.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: kuyu <kuyu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/01/18 15:59:26 by kuyu          #+#    #+#                 */
/*   Updated: 2026/01/18 18:07:11 by jdong         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "struct.h"
#include "libft.h"
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
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

/**
 * check if value = "", not create token node, but not failture
 * ft_substr might create empty string if it is only a stop symbol < > | 
 * 
 * return 0 if malloc failture;
 */
int	create_token_node(char *value, t_shell *shell, t_type type)
{
	t_token	*node;

	if (ft_strlen(value) == 0)
		return (1);
	node = (t_token *)malloc(sizeof(t_token));
	if (!node)
		return (ft_malloc_error("tokenization.\n", shell), 0);
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(node);
		return (ft_malloc_error("tokenization.\n", shell), 0);
	}
	node->type = type;
	node->next = NULL;
	append_to_lst(&(shell->token), node);
	return (1);
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
 * Maybe replace with ft_strchr(), but some compare logic need to be changed.
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
