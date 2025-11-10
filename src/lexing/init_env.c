#include "lex.h" //change to minishell.h after combin
#include "struct.h"
#include <stdlib.h>
#include "libft.h"
#include <stdio.h>		// for testing purpose

t_env	*create_node(char **key_value, t_shell *shell)
{
	t_env	*node;

	node = (t_env*)malloc(sizeof(t_env) * 1);
	if (!node)
	{
		free_split(key_value);
		ft_malloc_failure("Failture at malloc env.\n", shell);
	}
	node->key = ft_strdup(key_value[0]);
	if (key_value[1])
		node->value = ft_strdup(key_value[1]);
	else
		node->value = ft_strdup("");
	if (!node->key || !node->value)
	{
		free_split(key_value);
		ft_malloc_failure("Failure at malloc env.\n", shell);
	}
	node->next = NULL;
	return (node);
}

static void	append_to_lst(t_env **head, t_env *node)
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

t_env *init_env(char **envp, t_shell *shell)
{
	int		i;
	t_env	*node;		// store the newly created node
	char	**key_value;	// store the splitted result
	
	// TODO: what if envp is empty? 也许可以让ft_split在输入是“”和NULL时都返回NULL，来应对？
	i = 0;
	while (envp[i] != NULL)
	{
		key_value = ft_split(envp[i], '=');
		if (!key_value)
			ft_malloc_failure("Failture at malloc env.\n", shell);
		node = create_node(key_value, shell);
		append_to_lst(&(shell->env_lst), node);
		free_split(key_value);
		i++;
	}
	return (shell->env_lst);
}
