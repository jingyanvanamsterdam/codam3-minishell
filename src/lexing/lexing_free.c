#include <stdlib.h>
#include "lex.h"

void	free_split(char **key_value)
{
	int	i;

	if (!key_value)
		return ;
	i = 0;
	while (key_value[i])
	{
		free(key_value[i]);
		key_value[i] = NULL;
		++i;
	}
	free(key_value);
	key_value = NULL;
}

void	free_token_list(t_token *token)
{
	t_token	*tmp;
	while (token)
	{
		if (token->value)
			free(token->value);
		tmp = token->next;
		free(token);
		token = tmp;
	}
}