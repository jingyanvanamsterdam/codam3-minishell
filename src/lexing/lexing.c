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

t_token	*create_token_node(char **tmp, char *value, t_token *token, t_token_type type)
{
	t_token	*node;

	node = (t_token*)malloc(sizeof(t_token) * 1);
	if (!node)
	{
		free_split(tmp);
		ft_token_failure("Failture at malloc token node.\n", token);
	}
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free_split(tmp);
		ft_token_failure("Failure at malloc env.\n", token);
	}
	node->type = type;
	node->next = NULL;
	return (node);
}

//typedef enum e_lex_stauts
//{
//	DOUBLE_QUOTE,
//	SINGLE_QUOTE,
//	GENERAL
//}			t_lex_status;

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

t_token	*create_type_node(char **tmp, t_token *head, int i, int j)
{
	t_token	*node;
	char	type;

	type = tmp[i][j];
	node = NULL;
	if (type == '|')
		node = create_token_node(tmp, "|", head, PIPE);
	else if (type == '<')
	{
		if (tmp[i][j + 1] && tmp[i][j + 1] == '<')
			node = create_token_node(tmp, "<<", head, HEREDOC);
		else
			node = create_token_node(tmp, "<", head, REDIR_IN);
	}
	else if (type == '>')
	{
		if (tmp[i][j + 1] && tmp[i][j + 1] == '>')
			node = create_token_node(tmp, ">>", head, APPEND);
		else
			node = create_token_node(tmp, ">", head, REDIR_OUT);
	}
	return (node);
}

/**
 * malloc failure at 1. split: free env_list and exit
 */
t_token	*tokenization(char *input, t_token *head)
{
	int	i;
	int	start;
	int	end;
	char	**tmp;
	t_token	*node;
	t_token *prev_node;
	char *value;

	i = 0;
	tmp = ft_split(input, ' ');
	node = NULL;
	value = NULL;
	if (!tmp)
		ft_token_failure("Failture at malloc - tokenization\n", head);
	while (tmp[i])
	{
		//check the string what are the symbol;
		start = 0;
		end = 0;
		while (tmp[i][end])
		{
			if (tmp[i][end] == '|' || tmp[i][end] == '<' || tmp[i][end] == '>')
			{
				value = ft_substr(tmp[i], start, end);
				if (!value)
					ft_token_failure("Failture at malloc - tokenization\n", head);
				node = create_token_node(tmp, value, head, WORD);
				if (head == NULL)
					head = node;
				else
					prev_node->next = node;
				prev_node = node;
				free(value);
				value = NULL;
				node = create_type_node(tmp, head, i, end);
				prev_node->next = node;
				prev_node = node;
				end++;
				if (tmp[i][end] && (tmp[i][end] == '<' || tmp[i][end] == '>'))
					end++;
				start = end;
			}
			else if (tmp[i][end] == '\'')
			{
				// single quote flag
			}
			else if (tmp[i][end] == '\"')
			{
				// double quote flag
			}
			else
				end++;
		}
		i++;
	}
	free_split(tmp);
	return (head);
}

