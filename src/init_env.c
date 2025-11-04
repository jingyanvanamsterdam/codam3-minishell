#include "minishell.h"
#include "struct.h"
#include <stdlib.h>
#include "libft.h"

#include <stdio.h>		// for testing purpose

t_env	*create_node(char **key_value, t_env *env_list)
{
	t_env	*node;

	node = (t_env*)malloc(sizeof(t_env) * 1);
	if (!node)
	{
		free_split(key_value);
		ft_malloc_failure("Failture at malloc env.\n", env_list);
	}
	node->key = ft_strdup(key_value[0]);
	if (key_value[1])
		node->value = ft_strdup(key_value[1]);
	else
		node->value = "";
	if (!node->key || !node->value)
	{
		free_split(key_value);
		ft_malloc_failure("Failure at malloc env.\n", env_list);
	}
	node->next = NULL;
	return (node);
}

t_env *init_env(char **envp)
{
	int		i;
	t_env	*head;		// keep the first node of the list
	t_env	*node;		// store the newly created node
	t_env	*prev_node;
	char	**key_value;	// store the splitted result
	
	head = NULL;
	// TODO: what if envp is empty? 也许可以让ft_split在输入是“”和NULL时都返回NULL，来应对？
	i = 0;
	while (envp[i] != NULL)
	{
		key_value = ft_split(envp[i], '=');
		if (!key_value)
			ft_malloc_failure("Failture at malloc env.\n", head);
		node = create_node(key_value, head);
		if (i != 0)
			prev_node->next = node;
		else
			head = node;
		prev_node = node;
		free_split(key_value);
		i++;
	}
	return (head);
}
