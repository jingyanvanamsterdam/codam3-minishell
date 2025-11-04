#include <stdlib.h>
#include "minishell.h"
#include "env.h"

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
