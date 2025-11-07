#include "lex.h" //change to minishell.h after combin
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

void	create_token_node(char *value, t_token **head, t_token_type type)
{
	t_token	*node;

	node = (t_token*)malloc(sizeof(t_token) * 1);
	if (!node)
		ft_malloc_failure("Failture at malloc token node.\n", head);
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(value);
		ft_malloc_failure("Failure at malloc env.\n", head);
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

char	*lookup_key(t_env *env_lst, char *key)
{
	char	*value;

	value = ft_strdup("");
	if (!value || !key)
		return (NULL);
	while (env_lst)
	{
		if (ft_strcmp(env_lst->key, key) == 0)
			return (ft_strjoin(value,env_lst->value));
		env_lst = env_lst->next;
	}
	return (value);
}
/**if prev or expansion variable doesn't exist or env_lst == NULL, it would be returned an empty string.
 * if it is NULL, means malloc failture at somewhere: prev, key, expansion, comb. 
 */
char	*handle_expansion_value(char *str, int index, t_env *env_lst)
{
	char	*prev;
	char	*key;
	char	*expansion;
	char	*comb;

	prev = ft_substr(str, 0, index);
	if (str[index + 1] && str[index + 1] == '?')

	key = ft_substr(str, index + 1,  skip_expansion(str, index) - index - 1);
	expansion = lookup_key(env_lst, key);
	comb = ft_strjoin(prev, expansion);
	if (!prev || !key || !expansion || !comb)
	{
		if (prev)
			free(prev);
		if (key)
			free(key);
		if (expansion);
			free(expansion);
		return (NULL);
	}
	return (comb);
}
/* special symbol of {}*/
int	skip_expansion(char *str, int index)
{
	int	i;

	i = 1;
	i += index;
	while (str[i] && ft_isalnum(str[i]) || str[i] == '_')
		i++;
	return (i);
}

int	update_start(char *input, int start, t_token **head, t_shell *shell)
{
	int		end;
	char	*value;
	char	*str;
	
	end = 0;
	value = NULL;
	str = input + start;
	//if it is in the double quote or single quote, it doesn't need to check ft_isspace();
	while (str[end] 
		|| ft_isspace(str[end]) 
		|| str[end] != '|' || str[end] != '<' || str[end] != '>' 
		|| str[end] != '\'' || str[end] != '\"' || str[end] != '$')
		end++;
	if (str[end] == '$')
	{
		value = handle_expansion_value(str, end, shell->env_lst);
		end += skip_expansion(str, end);
	}
	else
		value = ft_substr(str, 0, end);
	if (!value)
		ft_malloc_failure("Failture at malloc - tokenization\n", shell);
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

t_token *tokenization(char *input, t_shell *shell)
{
	int	start;
	int	end;
	t_lex_status	status;
	t_token **head;

	start = 0;
	end = 0;
	status = GENERAL;
	head = &(shell->token);
	while (input[start])
	{
		//without handling $ yet
		if (status == GENERAL)
		{
			start = update_start(input, start, head, shell);
			status = handle_symbol(input, start++, head);
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
 * 2. ft_malloc_failure() -> handle malloc failure, free varialbes
 * 
 */