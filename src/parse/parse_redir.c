/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kuyu <kuyu@student.codam.nl>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 16:24:07 by kuyu              #+#    #+#             */
/*   Updated: 2026/01/18 16:26:45 by kuyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "struct.h"
#include "parse.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> // printf

static void	append_to_rdir_lst(t_redir **head, t_redir *node)
{
	t_redir	*tmp;

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
 * Return value = next token, if it is NULL or != WORD,
 * means after redir symbol, there is an input error.
 * Check !shell->token; ft_malloc_parsing clean up token list and cmd list
 * ft_input_error() will clean up shell's cmd, token. 
 */
t_token	*handle_redir(t_token *token, t_redir **redir, t_shell *shell)
{
	t_redir	*node;
	char	*value;
	t_type	t;

	value = token->value;
	t = token->type;
	token = token->next;
	if (!token || token->type != WORD)
		return (ft_input_error("near upexpected token `", value, shell), NULL);
	node = (t_redir *)malloc(sizeof(t_redir));
	if (!node)
		return (ft_malloc_parsing("parsing", shell), NULL);
	node->type = t;
	if (t == HEREDOC)
		node->file = ft_strdup(token->value);
	else
		node->file = remove_quote(token->value, shell, false);
	if (!node->file)
		return (ft_malloc_parsing("parsing", shell), NULL);
	node->next = NULL;
	append_to_rdir_lst(redir, node);
	return (token);
}
