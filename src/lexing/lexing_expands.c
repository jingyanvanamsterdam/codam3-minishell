#include "lex.h"
#include "struct.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> // for printf

size_t	expands_index(char *str, size_t end)
{
	size_t	i;
	
	i = 0;
	while (i < end)
	{
		if (str[i] == '$')
			return (i);
		i++;
	}
	return (i);
}

char	*lookup_key(t_env *env_lst, const char *key)
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

/** 
 * string begin the the index 0 of first char after $,
 * return the length of the name of expands */
size_t	skip_expansion(char *str)
{
	size_t	i;

	//skip the $
	i = 1;
	if (str[i] == '?')
		return (++i);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

/**
 * if expansion variable doesn't exist or env_lst == NULL, it would be returned an empty string.
 */
char	*expansion(char *str, t_shell *shell, size_t end)
{
	char	*expansion;
	char	*key;

	expansion = NULL;
	key = NULL;
	if (end == 0)
		expansion = ft_strdup("$");
	else if (str[1] == '?')
		expansion = ft_itoa(shell->prev_exit);
	else
	{
		key = ft_substr(str, 1, end);
		if (!key)
			return (NULL);
		expansion = lookup_key(shell->env_lst, key);
	}
	if (key)
		free(key);
	if (!expansion)
		return (NULL);
	//printf("expansion = %s\n", expansion);
	return (expansion);
}

char	*handle_expands(char *str, size_t i, size_t len, t_shell *shell)
{
	size_t	exp_len;
	size_t	prev;
	char	*res;

	exp_len = 0;
	res = ft_substr(str, 0, i);
	if (!res)
		return (NULL);
	prev = 0;
	while (i < len)
	{
		if (str[i] == '$')
		{
			exp_len = skip_expansion(str);
			res = append_to_str(res, expansion(str + i, shell, exp_len));
			if (!res)
				return (NULL);
			i += exp_len;
			prev = i;
		}
		else
			i++;
	}
	if (prev < i)
		res = append_to_str(res, ft_substr(str, prev, i - prev));
	if (!res)
		return (NULL);
	return (res);
}

///** Return a pointer which was the sub string before the expands and the expands.
// * Create a sub string from the start until the current index of "$" 
// * Append it to the previous string by copy and cat and create a new one.
// * Index skip the length of the name of expands varible.
// * Append the varible's string to the res.
// */
//char	*join_prev_expands(char *str, int start, int cur, t_shell *shell, char *res)
//{
//	int		exp_len;

//	exp_len = 0;
//	res = append_to_str(res, ft_substr(str, start, cur - start));
//	if (!res)
//		return (NULL);
//	exp_len = skip_expansion(str + cur);
//	res = append_to_str(res, expansion(str + cur + 1, shell, exp_len));
//	if (!res)
//		return (NULL);
//	return (res);
//}


///**
// * return the pointer of a value that contains all the expansion without space.
// */
//char	*check_expands(char *str, int len, t_shell *shell)
//{
//	int		i;
//	int		start;
//	char	*res;

//	i = 0;
//	res = NULL;
//	start = 0;
//	while (i < len)
//	{
//		if (str[i] == '$')
//		{
//			res = join_prev_expands(str, start, i, shell, res);
//			if (!res)
//				return (NULL);
//			i += skip_expansion(str);
//			start = i;
//		}
//		else
//			i++;
//	}
//	if (start < i)
//		res = append_to_str(res, ft_substr(str, start, i - start));
//	if (!res)
//		return (NULL);
//	return (res);
//}


