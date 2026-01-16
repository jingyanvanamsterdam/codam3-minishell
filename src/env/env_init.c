#include "minishell.h"
#include <stdlib.h>
#include "libft.h"

t_env	*create_node(char **key_value, t_shell *shell)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env) * 1);
	if (!node)
		return (ft_malloc_error("env setup", shell), NULL);
	node->key = ft_strdup(key_value[0]);
	if (key_value[1])
		node->value = ft_strdup(key_value[1]);
	else
		node->value = ft_strdup("");
	if (!node->key || !node->value)
		return (ft_malloc_error("env setup", shell), NULL);
	node->next = NULL;
	return (node);
}

void	append_to_env_lst(t_env **head, t_env *node)
{
	t_env	*tmp;

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

// TODO: need to handle env variable like LS_COLORS=rs=0:... properly
int	init_env(char **envp, t_shell *shell)
{
	int		i;
	t_env	*node;
	char	**key_value;

	i = 0;
	while (envp[i] != NULL)
	{
		key_value = ft_split(envp[i], '=');
		if (!key_value)
			return (ft_malloc_error("setup env key_value", shell), 0);
		node = create_node(key_value, shell);
		if (!node)
			return (free_2d_arr(key_value), 0);
		printf("DEBUG: key=%s, value_len=%zu\n", node->key, ft_strlen(node->value)); // Add this
		append_to_env_lst(&(shell->env_lst), node);
		free_2d_arr(key_value);
		i++;
	}
	if (envp[0] == NULL)
		printf("no envp\n");
	return (1);
}
