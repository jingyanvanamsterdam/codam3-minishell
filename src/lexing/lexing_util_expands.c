#include "lex.h" // change to minishell.h
#include "libft.h"
#include <stdlib.h> 

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
int	skip_expansion(char *str)
{
	int	i;

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

/** Return a pointer which was the sub string before the expands and the expands.
 * Create a sub string from the start until the current index of "$" 
 * Append it to the previous string by copy and cat and create a new one.
 * Index skip the length of the name of expands varible.
 * Append the varible's string to the res.
 */
char	*join_prev_expands(char *str, int start, int cur, t_shell *shell, char *res)
{
	int		exp_len;

	exp_len = 0;
	res = append_to_str(res, ft_substr(str, start, cur - start));
	if (!res)
		return (NULL);
	exp_len = skip_expansion(str + cur);
	res = append_to_str(res, expansion(str + cur + 1, shell, exp_len));
	if (!res)
		return (NULL);
	return (res);
}
