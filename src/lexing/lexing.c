#include "lex.h" //change to minishell.h after combin
#include "struct.h"
#include "libft.h"
#include <stdbool.h> // for boolean functions 
#include <stdio.h>	// for printf
//#include <stdlib.h>
//#include <signal.h>
//#define _GNU_SOURCE
//#include <readline/readline.h>
//#include <readline/history.h>
//#include <unistd.h> // for sleep

static void	append_to_lst(t_token **head, t_token *node)
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

char	*lookup_key(t_env *env_lst, const char *key)
{
	char	*value;

	value = ft_strdup("");
	if (!value || !key)
		return (NULL);
	if (ft_isdigit(key[0]))
		// should not interpret?
	while (env_lst)
	{
		if (ft_strcmp(env_lst->key, key) == 0)
			return (ft_strjoin(value,env_lst->value));
		env_lst = env_lst->next;
	}
	return (value);
}

/** 
 * string begin the the index 0 of first char after $,
 * return the length of the name of expands */
int	skip_expansion(char *str)
{
	int	i = 0;
	if (str[i] == '?')
		return (++i);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}
/**
 * if expansion variable doesn't exist or env_lst == NULL, it would be returned an empty string.
 */
char	*expansion(char *str, t_shell *shell, int end)
{
	char	*expansion;
	char	*key;

	expansion = NULL;
	key = NULL;
	if (end == 0)
		expansion = ft_strdup("$");
	else if (str[0] == '?')
		expansion = ft_itoa(shell->prev_exit);
	else
	{
		key = ft_substr(str, 0, end);
		if (!key)
			return (NULL);
		expansion = lookup_key(shell->env_lst, key);
	}
	if (key)
		free(key);
	if (!expansion)
		return (NULL);
	return (expansion);
}

char	*create_src(char *str, int start, int len)
{
	char	*content;

	content = NULL;
	content = ft_substr(str, start, len);
	if (!content)
		return (NULL);
	return (content);
}

char	*append_to_str(char *dst, char *src)
{
	size_t	dst_len;
	size_t	src_len;
	char	*new;

	if (!src)
		return (dst);
	dst_len = 0;
	if (dst)
		dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	new = malloc(dst_len + src_len + 1);
	if (!new)
	{
		if (dst)
			free(dst);
		return (free(src), NULL);
	}
	if (dst)
		ft_strlcpy(new, dst, dst_len + 1);
	else
		new[0] = '\0';
	ft_strlcat(new, src, src_len + 1);
	if (dst)
		free(dst);
	return (free(src), new);
}

int	find_end(char *str)
{
	int	end;
	
	end = 0;
	while (str[end] && !ft_isspace(str[end]) 
		&& str[end] != '|' && str[end] != '<' && str[end] != '>' 
		&& str[end] != '\'' && str[end] != '\"')
		end++;
	return (end);
}
/**
 * return the pointer of a value that contains all the expansion without space.
 */
char	*check_expands(char *str, int len, t_shell *shell)
{
	int		i;
	int		expands;
	int		exp_start;
	char	*res;

	i = 0;
	expands = 0;
	res = NULL;
	exp_start = 0;
	while (i < len)
	{
		if (str[i] == '$')
		{
			res = append_to_str(res, create_src(str, exp_start, i - exp_start));
			if (!res)
				return (NULL);
			expands = skip_expansion(str + i + 1);
			res = append_to_str(res, expansion(str + i + 1, shell, expands));
			if (!res)
				return (NULL);
			i += expands + 1;
			exp_start = i;
		}
		else
			i++;
	}
	if (exp_start < i)
		res = append_to_str(res, create_src(str, exp_start, i - exp_start));
	if (!res)
		return (NULL);
	return (res);
}

bool	contain_expands(char *str, int end)
{
	while (end >= 0)
	{
		if (str[end] == '$')
			return (true);
		end--;
	}
	return (false);
}

int	update_start(char *input, int start, t_shell *shell)
{
	int		end;
	char	*value;
	char	*str;
	
	end = 0;
	value = NULL;
	str = input + start;
	//if it is in the double quote or single quote, it doesn't need to check ft_isspace();
	end = find_end(str);
	if (contain_expands(str, end - 1))
		value = check_expands(str, end, shell);
	else
		value = ft_substr(str, 0, end++);
	if (!value)
		ft_malloc_failure("Failture at malloc - tokenization\n", shell);
	create_token_node(value, shell, WORD);
	free(value);
	return (end);
}

//t_lex_status handle_symbol(char *input, int start, t_shell *shell)
//{
//	char *str;

//	str = input + start;
//	if (input[start] == '|' || input[start] == '<' || input[start] == '>')
//	{
//		//handle special symbol and add token
//		return (GENERAL);
//	}
//	if (input[start] == '\'')
//		return (SINGLE_QUOTE);
//	if (input[start] == '\"')
//		return (DOUBLE_QUOTE);
//}

t_token *tokenization(char *input, t_shell *shell)
{
	int	start;
	t_lex_status	status;

	start = 0;
	status = GENERAL;
	while (input[start])
	{
		//without handling $ yet
		if (status == GENERAL)
		{
			start += update_start(input, start, shell);
			printf("start = %d\n", start);
			//status = handle_symbol(input, start - 1, shell);
			if (input[start] == '<' || input[start] == '>')
				start++;
		}
		else if (status == DOUBLE_QUOTE)
		{
			// doing something.
			status = GENERAL;
		}
		else if (status == SINGLE_QUOTE)
		{
			//takes everything as one token.
			status = GENERAL;
		}
	}
	return (shell->token);
}

/**
 * To do:
 * 1. Different status handling string
 * 
 */