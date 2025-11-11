#include <stdlib.h>
#include "lex.h" //change to minishell.h after combin

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

void	free_token_list(t_token **lst)
{
	t_token	*temp;

	if (!lst)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->value)
			free((*lst)->value);
		free(*lst);
		*lst = temp;
	}
}

void	free_env_lst(t_env **lst)
{
	t_env	*temp;

	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->key)
			free((*lst)->key);
		if ((*lst)->value)
			free((*lst)->value);
		free(*lst);
		*lst = temp;
	}
}
// more to free later
void	free_shell(t_shell *shell)
{
	if (shell->token)
		free_token_list(&(shell->token));
	if (shell->env_lst)
		free_env_lst(&(shell->env_lst));
	free(shell);
}