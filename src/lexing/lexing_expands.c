#include "parse.h" 
#include "struct.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> // for printf

/** 
 */
char	*process_expands(char *str, size_t *i, size_t *prev, t_shell *shell)
{
	char	*res;
	size_t	exp_len;

	res = NULL;
	res = append_to_str(res, ft_substr(str, *prev, (*i) - (*prev)));
	if (!res)
		return (NULL);
	exp_len = skip_expansion(str + (*i));;
	res = append_to_str(res, expansion(str + (*i), shell, exp_len - 1));
	if (!res)
		return (NULL);
	(*i) += exp_len;
	*prev = *i;
	return (res);
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
			return (ft_strjoin(value, env_lst->value));
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
 * end = length of the name of the expands. expands length contains 1 as $. 
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
	return (expansion);
}

char	*handle_expands(char *str, size_t len, t_shell *shell)
{
	char	*res;
	char	*tmp;
	size_t	prev;
	size_t	i;

	prev = 0;
	i = 0;
	res = NULL;
	while (i < len)
	{
		if (str[i] == '$')
		{
			tmp = process_expands(str, &i, &prev, shell);
			if (!tmp)
				return (NULL);
			res = append_to_str(res, tmp);
			if (!res)
				return (NULL);
		}
		else
			i++;
	}
	if (prev < len)
		res = append_to_str(res, ft_substr(str, prev, len - prev));
	return (res);
}
