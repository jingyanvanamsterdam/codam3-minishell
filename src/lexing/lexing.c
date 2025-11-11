#include "lex.h" //change to minishell.h after combin
#include "struct.h"
#include "libft.h"
#include <stdbool.h> // for boolean functions 
#include <stdio.h>	// for printf
#include <stdlib.h>

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

int	find_end(char *str, t_lex_status *status)
{
	int	end;
	
	end = 0;
	if (*status == GENERAL)
	{
		while (str[end] && !ft_isspace(str[end]) 
		&& str[end] != '|' && str[end] != '<' && str[end] != '>' 
		&& str[end] != '\'' && str[end] != '\"')
		end++;
	}
	else if (*status == DOUBLE_QUOTE)
	{
		while (str[end] && str[end] != '\"')
			end++;
	}
	else if (*status == SINGLE_QUOTE)
	{
		while (str[end] && str[end != '\''])
			end++;
	}
	//printf("end at %c, end = %d\n", str[end], end);
	return (end);
}

int	update_start(char *input, int start, t_shell *shell, t_lex_status *status)
{
	int		end;
	char	*value;
	char	*str;
	
	end = 0;
	value = NULL;
	str = input + start;
	//printf("input at start = %c\n", str[0]);
	//if it is in the double quote or single quote, it doesn't need to check ft_isspace();
	end = find_end(str, status);
	printf("after find end = %d\n", end);
	if (contain_expands(str, end - 1))
		value = check_expands(str, end, shell);
	else
		value = ft_substr(str, 0, end);
	if (!value)
		ft_malloc_failure("Failture at malloc - tokenization\n", shell);
	create_token_node(value, shell, WORD);
	free(value);
	return (end);
}

size_t	handle_special_symbol(char *str, size_t end, t_shell *shell)
{
	if (str[end] == '|')
		create_token_node("|", shell, PIPE);
	else if (str[end] == '>')
	{
		if (str[end + 1] && str[end + 1] == '>')
		{
			create_token_node(">>", shell, APPEND);
			end++;
		}
		else
			create_token_node(">", shell, REDIR_OUT);
	}
	else if (str[end] == '<')
	{
		if (str[end + 1] && str[end + 1] == '<')
		{
			create_token_node("<<", shell, HEREDOC);
			end++;
		}
		else
			create_token_node("<", shell, REDIR_IN);
	}
	return (++end);
}

size_t	skip_space_or_quotes(char *str, size_t end, t_lex_status *status)
{
	if (str[end])
	{
		// should be space, or \" or \'
		while(ft_isspace(str[end]))
			end++;
		if (str[end] == '\'')
		{
			*status = SINGLE_QUOTE;
			while(ft_isspace(str[++end]))
				end++;
		}
		else if (str[end] == '\"')
		{
			*status = DOUBLE_QUOTE;
			while(ft_isspace(str[++end]))
				end++;
		}
	}
	return (end);
}

size_t	update_sq_start(char *input, size_t start, t_shell *shell, t_lex_status *status)
{
	char	*str;
	size_t	end;
	char	*value;

	str = input + start;
	end = 0;
	value = NULL;
	while (str[end] && str[end] != '\'')
		end++;
	if (str[end] == '\'')
	{
		*status = GENERAL;
		value = malloc(end + 1);
		if (!value)
			ft_malloc_failure("Failure of malloc at create token node\n", shell);
		create_token_node(value, shell, WORD);
		free(value);
		return (end);
	}
	else
	{
		// unclosed quote. error;
		return (end);
	}
}

t_token *tokenization(char *input, t_shell *shell)
{
	size_t	start;
	t_lex_status	status;

	start = 0;
	status = GENERAL;
	size_t	input_len = ft_strlen(input);
	while (start < input_len)
	{
		//printf("input len = %zu\n", input_len);
		if (status == GENERAL)
		{
			start += update_start(input, start, shell, &status);
			printf("after update start %zu\n", start);
			if (input[start] == '|' || input[start] == '<' || input[start] == '>')
				start = handle_special_symbol(input, start, shell);
			else
				start = skip_space_or_quotes(input, start, &status);
			printf("after handle symbol start = %zu\n", start);
		}
		else if (status == DOUBLE_QUOTE)
		{
			//start += update_dq_start(input, start, shell);
			status = GENERAL;
		}
		else if (status == SINGLE_QUOTE)
		{
			//start += update_sq_start(input, start, shell, &status);
		}
	}
	return (shell->token);
}

/**
 * To do:
 * 1. Different status handling string
 * 
 */