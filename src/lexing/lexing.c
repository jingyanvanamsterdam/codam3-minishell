#include "lex.h"
#include "struct.h"
#include "libft.h"
#include <stdbool.h> // for boolean functions 
//#include <stdio.h>	
//#include <stdlib.h>
//#include <signal.h>
//#define _GNU_SOURCE
//#include <readline/readline.h>
//#include <readline/history.h>
//#include <unistd.h> // for sleep

void	append_to_lst(t_token **head, t_token *node)
{
	t_token	*tmp;

	//if (!head || !node)
	//	return ;
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

void	create_token_node(char *value, t_token **head, t_token_type type)
{
	t_token	*node;

	node = (t_token*)malloc(sizeof(t_token) * 1);
	if (!node)
		ft_token_failure("Failture at malloc token node.\n", head);
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(value);
		ft_token_failure("Failure at malloc env.\n", head);
	}
	node->type = type;
	node->next = NULL;
	append_to_lst(head, node);
}

typedef enum e_lex_stauts
{
	DOUBLE_QUOTE,
	SINGLE_QUOTE,
	GENERAL
}			t_lex_status;

//bool	switch_status(char c, t_lex_status *status)
//{
//	if (c == '\"')
//	{
//		if (*status == GENERAL)
//			*status = DOUBLE_QUOTE;
//		else if (*status == DOUBLE_QUOTE)
//			*status = GENERAL;
//		else
//			return (false);
//	}
//	else if (c == '\'')
//	{
//		if (*status == GENERAL)
//			*status = SINGLE_QUOTE;
//		else if (*status == SINGLE_QUOTE)
//			*status = GENERAL;
//		else
//			return (false);
//	}
//	else
//		return (false);
//	return (true);
//}

//t_token	*create_type_node(char **tmp, t_token **head, int i, int j)
//{
//	t_token	*node;
//	char	type;

//	type = tmp[i][j];
//	node = NULL;
//	if (type == '|')
//		node = create_token_node(tmp, "|", head, PIPE);
//	else if (type == '<')
//	{
//		if (tmp[i][j + 1] && tmp[i][j + 1] == '<')
//			node = create_token_node(tmp, "<<", head, HEREDOC);
//		else
//			node = create_token_node(tmp, "<", head, REDIR_IN);
//	}
//	else if (type == '>')
//	{
//		if (tmp[i][j + 1] && tmp[i][j + 1] == '>')
//			node = create_token_node(tmp, ">>", head, APPEND);
//		else
//			node = create_token_node(tmp, ">", head, REDIR_OUT);
//	}
//	return (node);
//}

int	update_start(char *input, int start, t_token **head)
{
	int		end;
	char	*value;
	char	*str;
	
	end = 0;
	value = NULL;
	str = input + start;
	while (str[end] 
		|| ft_isspace(str[end]) 
		|| str[end] != '|' || str[end] != '<' || str[end] != '>' 
		|| str[end] != '\'' || str[end] != '\"')
		end++;
	value = ft_substr(str, 0, end);
	if (!value)
		ft_token_failure("Failture at malloc - tokenization\n", head);
	create_token_node(value, head, WORD);
	free(value);
	return (end);
}

t_lex_status handle_symbol(char *input, int start, t_token **head)
{
	char *str;

	str = input + start;
	if (input[start] == '|' || input[start] == '<' || input[start] == '>')
	{
		//handle special symbol and add token
		return (GENERAL);
	}
	if (input[start] == '\'')
		return (SINGLE_QUOTE);
	if (input[start] == '\"')
		return (DOUBLE_QUOTE);
}

t_token *tokenization(char *input, t_token *head)
{
	int	start;
	int	end;
	t_lex_status	status;

	start = 0;
	end = 0;
	status = GENERAL;
	while (input[start])
	{
		//without handling $ yet
		start = update_start(input, start, &head);
		if (status == GENERAL)
		{
			status = handle_symbol(input, start++, &head);
			if (input[start] == '<' || input[start] == '>')
				start++;
		}
		else if (status == DOUBLE_QUOTE || status == SINGLE_QUOTE)
		{
			// doing something.
			status = GENERAL;
		}
	}
}
