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

/**
 * This function copy the dst string and cat the src string into a new string and free the dst and src, return the new pointer. 
 */
char	*append_to_str(char *dst, char *src)
{
	size_t	dst_len;
	size_t	src_len;
	char	*new;

	if (!src)
		return (free(dst), NULL);
	dst_len = 0;
	if (dst)
		dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	new = malloc(dst_len + src_len + 1);
	if (!new)
		return (free(dst), free(src), NULL);
	if (dst)
		ft_strlcpy(new, dst, dst_len + 1);
	else
		new[0] = '\0';
	ft_strlcat(new, src, dst_len + src_len + 1);
	return (free(dst), free(src), new);
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

/**
 * This func return the index of the end of a string/
 * Based on status, the end requirement is different.
 */
size_t	find_end(char *str, t_lex_status *status)
{
	size_t	end;
	
	end = 0;
	if (*status == GENERAL)
		while (str[end] && !ft_isspace(str[end]) 
		&& str[end] != '|' && str[end] != '<' && str[end] != '>')
		end++;
	else if (*status == DOUBLE_QUOTE)
		while (str[end] && str[end] != '\"')
			end++;
	else
		while (str[end] && str[end] != '\'')
			end++;
	//printf("end at %c, end = %d\n", str[end], end);
	return (end);
}
size_t	quote_index(char *str, size_t end, t_lex_status *status)
{
	size_t i;

	i = 0;
	while (i < end)
	{
		if (str[i] == '\'')
		{
			*status = SINGLE_QUOTE;
			return (i);
		}
		else if (str[i] == '\"')
		{
			*status = DOUBLE_QUOTE;
			return (i);
		}
		i++;
	}
	return (i);
}
size_t	update_start(char *str, t_shell *shell, t_lex_status *status)
{
	size_t	end;
	size_t	quote_i;
	size_t	expands_i;
	char	*value;
	char	*src;

	end = 0;
	value = NULL;
	end = find_end(str, status);
	quote_i = quote_index(str, end, status);
	expands_i = contain_expands(str, end);
	if (quote_i > expands_i)
	{
		value = ft_substr(str, 0, expands_i);
		if (!value)
			ft_malloc_failure("Failture at malloc - tokenization\n", shell);
		src = handle_expands(str + expands_i, quote_i - expands_i, shell);
	}
	else
	{
		value = ft_substr(str, 0, quote_i);
		if (!value)
			ft_malloc_failure("Failture at malloc - tokenization\n", shell);
		src = handle_quote(str + quote_i, end, shell);
	}
	if (!src)
		ft_malloc_failure("Failture at malloc - tokenization\n", shell);
	value = append_to_str(value, src);
	if (!value)
		ft_malloc_failure("Failture at malloc - tokenization\n", shell);
	create_token_node(value, shell, WORD);
	free(value);
	free(src);
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
			start += update_start(input + start, shell, &status);
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
	if (status != GENERAL)
		ft_input_error("unclosed quote\n", shell);
	return (shell->token);
}

/**
 * To do:
 * 1. Different status handling string
 * 
 */