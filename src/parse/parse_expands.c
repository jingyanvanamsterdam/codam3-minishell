#include "parse.h" 
#include "struct.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h> // for printf

char	*lookup_key(t_env *env_lst, const char *key)
{
	char	*value;
	char	*tmp;

	value = ft_strdup("");
	if (!value || !key)
		return (NULL);
	while (env_lst)
	{
		if (ft_strcmp(env_lst->key, key) == 0)
		{
			tmp = value;
			value = ft_strjoin(tmp, env_lst->value);
			free_charptr(&tmp);
			return (value);
		}
		env_lst = env_lst->next;
	}
	return (value);
}

/**
 * end = length of the name of the expands. expands length contains 1 as $. 
 * if expansion variable doesn't exist or env_lst == NULL, it would be returned an empty string.
 * Here will handle $? and anything after ? will be conected with the exit code unless there is space.
 */
char	*expansion(char *str, t_shell *shell, size_t end)
{
	char	*expansion;
	char	*key;

	expansion = NULL;
	key = NULL;
	key = ft_substr(str, 1, end - 1);
	if (!key)
		return (NULL);
	expansion = lookup_key(shell->env_lst, key);
	if (key)
		free_charptr(&key);
	if (!expansion)
		return (NULL);
	return (expansion);
}

/** return -1 meaans malloc fails */
size_t	handle_expands(char *str, t_shell *shell, t_quotok **tok)
{
	char	*value;
	size_t	end;

	value = NULL;
	end = 1;
	if (!str[end] 
		|| (!ft_isalnum(str[end]) && str[end] != '_' && str[end] != '?'))
		value = ft_strdup("$");
	else if (str[end] == '?')
	{
		value = ft_itoa(shell->prev_exit);
		end++;
	}
	else
	{
		while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))
			end++;
		value = expansion(str, shell, end);
	}
	if (!value)
		return ((size_t)-1);
	if (create_quotok_node(value, tok) == -1)
		return (free_charptr(&value), (size_t)-1);
	free_charptr(&value);
	return (end);
}
