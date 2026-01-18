/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 16:23:53 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 16:32:30 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

/**
 * change status if the index 0 is a quote
 * The status will change back if the status is not 
 * General and the end index is quote.
 * then end + 1 to skip the close quote
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
		else if (str[0] == '\"')
			*status = DOUBLE_QUOTE;
	}
	return (start);
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
	}
	else if (*status == DOUBLE_QUOTE)
	{
		while (str[i] && str[i] != '\"')
			i++;
	}
	else
	{
		while (str[i] && str[i] != '\'' && str[i] != '\"')
			i++;
	}
	return (i);
}

/** return -1 if malloc failed */
int	create_quotok_node(char *value, t_quotok **head)
{
	t_quotok	*node;

	node = (t_quotok *)malloc(sizeof(t_quotok));
	if (!node)
		return (-1);
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(node);
		return (-1);
	}
	node->next = NULL;
	append_to_lst(head, node);
	return (1);
}
